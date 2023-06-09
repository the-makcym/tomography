#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>
#include "utilities.h"
#include "globals.h"

using namespace std;


// Конструктор класса конфигурационной информации
Config::Config() {
    ifstream config_buff;
    string line;

    config_buff.open(CONFIG_FILE);
    if (config_buff.is_open()) {
        while (getline(config_buff, line)) {
            // если строка - комментарий (содержит #)
            if (line.find('#') != string::npos) continue;

            size_t eq_pos = line.find('=');
            string key = line.substr(0, eq_pos);
            int value = stoi(line.substr(eq_pos + 1, line.length() - eq_pos));

            if (key == "NRHO") n_rho = value;
            else if (key == "NPHI") n_phi = value;
            else if (key == "NX") n_x = value;
            else if (key == "NY") n_y = value;
            else if (key == "INTERSECTIONS") intersections = (bool)value;
        }
    }
    config_buff.close();
}


// Решение квадратного уравнения - возвращает массив,
// заполненный сначала его корнями в порядке убывания, затем значениями NOREAL
Vector quadeq(real A, real B, real C) {
    if real_eq(A, 0) {
        assert(!real_eq(B, 0));
        return {-C / B};
    }

    real D = B * B - 4 * A * C;
    if real_eq(D, 0) {
        return {-B / 2 / A};
    }

    Vector roots;
    D = sqrt(D);
    roots[0] = (-B - D) / 2 / A;
    roots[1] = (-B + D) / 2 / A;
    if (roots[0] < roots[1]) {
        swap(roots[0], roots[1]);
    }
    return roots;
}


real standard_deviation(string &filename, Matrix &exact, Matrix &model) {
    filename = string("dat/") + filename + string(".dat");
    real numerator = 0, denominator = 0;

    for (int y = 0; y < exact.size(); ++y) {
        for (int x = 0; x < exact[0].size(); ++x) {
            numerator += pow(exact.at(y).at(x) - model.at(y).at(x), 2);
//            cout << numerator << ' ';
            denominator += pow(exact.at(y).at(x), 2);
        }
//        cout << '\n';
    }
    real sd = sqrt(numerator / denominator);

    ofstream dat;
    dat.open(filename.data());
    dat << sd;
    dat.close();

    return sd;
}


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void make_jpg_dat(string &filename, Matrix &data) {
    string img_file("img/");
    img_file = (img_file + filename).append(".jpg");

    string dat_file("dat/");
    dat_file = (dat_file + filename).append(".dat");
    ofstream dat;
    dat.open(dat_file);

    unsigned char ndata[data.size()][data[0].size()];
    real m = 0;

    for (int y = 0; y < data.size(); ++y) {
        for (int x = 0; x < data[0].size(); ++x) {
            m = max(data[y][x], m);
        }
    }
    for (int y = 0; y < data.size(); ++y) {
        for (int x = 0; x < data[0].size(); ++x) {
            ndata[y][x] = 255.f * data[y][x] / m;
            dat << data[y][x] << ' ';
        }
        dat << '\n';
    }
    stbi_write_jpg(img_file.data(), data[0].size(), data.size(), 1, ndata, 100);
    dat.close();
}
