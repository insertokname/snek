#include "path.hpp"

namespace snek {
    std::vector<std::vector<int>> gen_path(std::size_t height, std::size_t width) {
        std::vector<std::vector<int>> out(height, std::vector<int>(width));

        int index = 0;


        //we generate a hamiltonian cycle like this:
        // * - * - * - * - * - *
        // |                   |
        // *   * - *   * - *   *
        // |   |   |   |   |   |
        // * - *   * - *   * - *
        if (width % 2 == 0) {
            for (int i = height - 1;i >= 0;i--) {
                out[i][0] = index++;
            }
            for (int j = 1;j < width;j++) {
                out[0][j] = index++;
            }

            int j = width - 1;
            int i = 1;
            while (j > 0) {
                if (i == 1) {
                    for (;i < height;i++) {
                        out[i][j] = index++;
                    }
                    i = height - 1;
                }
                else {
                    for (;i >= 1;i--) {
                        out[i][j] = index++;
                    }
                    i = 1;
                }
                j--;
            }
        }

        //generating this cycle:
        // * - * - * - * - *
        // |               |
        // *   * - * - * - *
        // |   |
        // *   * - * - * - *
        // |               |
        // *   * - * - * - *
        // |   |
        // *   * - * - * - *
        // |               |
        // * - * - * - * - *
        else if (height % 2 == 0) {

            for (int j = width - 1;j >= 0;j--) {
                out[height - 1][j] = index++;
            }
            for (int i = height - 2;i >= 0;i--) {
                out[i][0] = index++;
            }

            int j = 1;
            int i = 0;
            while (i < height - 1) {
                if (j == 1) {
                    for (;j < width;j++) {
                        out[i][j] = index++;
                    }
                    j = width - 1;
                }
                else {
                    for (;j >= 1;j--) {
                        out[i][j] = index++;
                    }
                    j = 1;
                }
                i++;
            }
        }
        else {
            std::cout << "Both sides of the board are odd!\n"
                << "Can't generate hamiltonian cycle!\n"
                << "Exiting!\n";
            exit(1);
        }
        return out;
    }
}