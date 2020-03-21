#include <cstdlib>
#include <iostream>
#include <cstring>

const int BUF_SIZE = 8192;

int out(const char* output) {
    size_t bytes_written = fwrite(output, sizeof(char), strlen(output), stdout);
    if (bytes_written == 0) {
        perror("fwrite failed");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " <filename>\n";
        return EXIT_FAILURE;
    }

    FILE* f = fopen(argv[2], "r");
    if (!f) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    size_t prev_ind = BUF_SIZE;
    char prev_buf[BUF_SIZE];


    for (;;) {
        char buffer[BUF_SIZE];
        size_t bytes_read = fread(buffer, sizeof(char), std::size(buffer), f);

        if (bytes_read == 0) {
            if (ferror(f))
            {
                perror("fread failed");
                fclose(f);
                return EXIT_FAILURE;
            }

            break;
        }

        while (prev_ind != BUF_SIZE && BUF_SIZE + bytes_read >= strlen(argv[1]) + prev_ind) {
            bool flag = true;
            for (size_t i = 0; i < strlen(argv[1]); i++) {
                size_t pointer = prev_ind + i;
                if (pointer < BUF_SIZE) {
                    if (prev_buf[pointer] != argv[1][i]) {
                        flag = false;
                        break;
                    }
                } else {
                    if (buffer[pointer % BUF_SIZE] != argv[1][i]) {
                        flag = false;
                        break;
                    }
                }
            }
            if (flag) {
                fclose(f);
                return out("True\n");
            }
            prev_ind++;
        }

        for (size_t i = 0; i + strlen(argv[1]) <= bytes_read; i++) {
            bool flag = true;
            for (size_t j = 0; j < strlen(argv[1]); j++) {
                if (buffer[i + j] != argv[1][j]) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                fclose(f);
                return out("True\n");
            }
        }

        prev_ind = BUF_SIZE - (int)strlen(argv[1]) + 1;
        memcpy(prev_buf, buffer, BUF_SIZE);
    }

    fclose(f);
    return out("False\n");
}