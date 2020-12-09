import os


def compress(data, f_comp):
    dictionary = dict([(bytes([i]), i) for i in range(256)])
    max_index = 256
    bit_width = 9
    key = bytes()
    packed_bytes = ""

    for symbol in data:
        curr_byte = bytes([symbol])
        if key + curr_byte in dictionary:
            key += curr_byte
        else:
            bin_value = bin(dictionary[key])[2:]
            packed_bytes += '0' * (bit_width - len(bin_value)) + bin_value
            dictionary[key + curr_byte] = max_index
            if len(bin(max_index)[2:]) > bit_width:
                bit_width += 1
            max_index += 1
            key = curr_byte

    bin_value = bin(dictionary[key])[2:]
    packed_bytes += '0' * (bit_width - len(bin_value)) + bin_value

    if len(packed_bytes) % 8 != 0:
        packed_bytes += '0' * (8 - len(packed_bytes) % 8)

    output = bytearray()
    for symbol in range(0, len(packed_bytes), 8):
        output.append(int(packed_bytes[symbol:symbol + 8], 2))

    f_comp.write(output)


def decompress(data, f_decomp):
    dictionary = dict([(i, bytes([i])) for i in range(256)])
    next_index = 256
    bit_width = 9
    position = 0
    result = bytearray()

    data_bin_str = ""
    for i in data:
        bin_i = bin(i)[2:]
        data_bin_str += '0' * (8 - len(bin_i)) + bin_i

    key = int(data_bin_str[position:position + bit_width], 2)
    sequence = dictionary[key]
    result.extend(sequence)
    position += bit_width

    while position + bit_width <= len(data_bin_str):
        key = int(data_bin_str[position:position + bit_width], 2)
        if key in dictionary:
            current = dictionary[key]
        elif key == next_index:
            current = sequence + bytes([sequence[0]])

        result.extend(current)
        dictionary[next_index] = sequence + bytes([current[0]])
        next_index += 1
        position += bit_width
        if len(bin(next_index)[2:]) > bit_width:
            bit_width += 1

        sequence = current

    f_decomp.write(result)


def main():
    filename = 'test2'
    file_compressed = 'lzw_' + filename
    file_decompressed = 'dec_' + filename

    with open(filename, 'rb') as f, open(file_compressed, 'wb') as f_comp:
        content = f.read()
        compress(content, f_comp)

    with open(file_compressed, 'rb') as f_comp, open(file_decompressed, 'wb') as f_decomp:
        content = f_comp.read()
        decompress(content, f_decomp)

    print("Original file: '{}' - {} bytes".format(filename, os.path.getsize(filename)))
    print("Compressed file: '{}' - {} bytes".format(file_compressed, os.path.getsize(file_compressed)))
    print("Decompressed file: '{}' - {} bytes".format(file_decompressed, os.path.getsize(file_decompressed)))


if __name__ == '__main__':
    main()
