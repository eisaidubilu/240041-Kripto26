/*
Nama        : Syifa Azzahra
NPM         : 140810240041
Deskripsi   : Algoritma Kriptografi Hill Cipher*/

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <numeric>

using namespace std;

//fungsi bantu
int hurufKeAngka(char c) {
    return c - 'A';
}

char angkaKeHuruf(int val) {
    val = val % 26;
    if (val < 0) val += 26;
    return static_cast<char>(val + 'A');
}

string bersihkanTeks(string teks) {
    string bersih = "";
    for (char c : teks) {
        if (isalpha(c)) {
            bersih += toupper(c);
        }
    }
    return bersih;
}

int inversModulo(int num, int mod = 26) {
    num = (num % mod + mod) % mod;
    for (int i = 1; i < mod; i++) {
        if ((num * i) % mod == 1) return i;
    }
    return -1;
}

//operasi matriks
int hitungDet(int mat[2][2]) {
    return (mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]);
}

bool validasiKey(int mat[2][2]) {
    int det = (hitungDet(mat) % 26 + 26) % 26;
    return std::gcd(det, 26) == 1;
}

bool inversMatriks(int mat[2][2], int inv[2][2]) {
    int det = hitungDet(mat);
    int invDet = inversModulo(det);

    if (invDet == -1) return false;

    inv[0][0] = (mat[1][1] * invDet) % 26;
    inv[0][1] = (-mat[0][1] * invDet) % 26;
    inv[1][0] = (-mat[1][0] * invDet) % 26;
    inv[1][1] = (mat[0][0] * invDet) % 26;

    for (int r = 0; r < 2; r++) {
        for (int c = 0; c < 2; c++) {
            if (inv[r][c] < 0) inv[r][c] += 26;
        }
    }
    return true;
}

void inputKey(int key[2][2]) {
    while (true) {
        cout << "\nMasukkan matriks key 2x2:\n";
        cout << "K[0][0] = "; cin >> key[0][0];
        cout << "K[0][1] = "; cin >> key[0][1];
        cout << "K[1][0] = "; cin >> key[1][0];
        cout << "K[1][1] = "; cin >> key[1][1];

        cout << "Determinan = " << hitungDet(key) << endl;

        if (validasiKey(key)) {
            cout << "Key valid.\n";
            break;
        }
        cout << "Key tidak valid (tidak punya invers mod 26). Coba lagi.\n";
    }
}

//enkripsi dekripsi
string prosesHill(string input, int key[2][2], bool isDekripsi = false) {
    string teks = bersihkanTeks(input);
    if (teks.length() % 2 != 0) teks += 'X';

    int mOper[2][2];
    if (isDekripsi) {
        inversMatriks(key, mOper);
    } else {
        for (int r = 0; r < 2; r++)
            for (int c = 0; c < 2; c++)
                mOper[r][c] = key[r][c];
    }

    string hasil = "";
    for (size_t i = 0; i < teks.length(); i += 2) {
        int p1 = hurufKeAngka(teks[i]);
        int p2 = hurufKeAngka(teks[i + 1]);

        int c1 = (mOper[0][0] * p1 + mOper[0][1] * p2) % 26;
        int c2 = (mOper[1][0] * p1 + mOper[1][1] * p2) % 26;

        hasil += angkaKeHuruf(c1);
        hasil += angkaKeHuruf(c2);
    }
    return hasil;
}

//cari key
bool cariKey(string pInput, string cInput, int key[2][2]) {
    string pTeks = bersihkanTeks(pInput);
    string cTeks = bersihkanTeks(cInput);

    size_t len = min(pTeks.length(), cTeks.length());
    if (len < 4) {
        cout << "Teks minimal 4 huruf.\n";
        return false;
    }

    vector<pair<int, int>> pBlok, cBlok;
    for (size_t i = 0; i < len - 1; i += 2) {
        pBlok.push_back({hurufKeAngka(pTeks[i]), hurufKeAngka(pTeks[i + 1])});
        cBlok.push_back({hurufKeAngka(cTeks[i]), hurufKeAngka(cTeks[i + 1])});
    }

    // Metode 1: Perkalian Matriks (K = C x P^-1)
    for (size_t i = 0; i < pBlok.size(); i++) {
        for (size_t j = i + 1; j < pBlok.size(); j++) {
            int P[2][2] = {
                {pBlok[i].first, pBlok[j].first},
                {pBlok[i].second, pBlok[j].second}
            };
            int C[2][2] = {
                {cBlok[i].first, cBlok[j].first},
                {cBlok[i].second, cBlok[j].second}
            };

            int pInv[2][2];
            if (inversMatriks(P, pInv)) {
                int tempKey[2][2];
                tempKey[0][0] = (C[0][0] * pInv[0][0] + C[0][1] * pInv[1][0]) % 26;
                tempKey[0][1] = (C[0][0] * pInv[0][1] + C[0][1] * pInv[1][1]) % 26;
                tempKey[1][0] = (C[1][0] * pInv[0][0] + C[1][1] * pInv[1][0]) % 26;
                tempKey[1][1] = (C[1][0] * pInv[0][1] + C[1][1] * pInv[1][1]) % 26;

                for (int r = 0; r < 2; r++)
                    for (int c = 0; c < 2; c++)
                        if (tempKey[r][c] < 0) tempKey[r][c] += 26;

                if (validasiKey(tempKey)) {
                    for (int r = 0; r < 2; r++)
                        for (int c = 0; c < 2; c++)
                            key[r][c] = tempKey[r][c];
                    return true;
                }
            }
        }
    }

    // Metode 2: Fallback Brute Force
    for (int k00 = 0; k00 < 26; k00++) {
        for (int k01 = 0; k01 < 26; k01++) {
            for (int k10 = 0; k10 < 26; k10++) {
                for (int k11 = 0; k11 < 26; k11++) {
                    int tesKey[2][2] = {{k00, k01}, {k10, k11}};
                    if (validasiKey(tesKey)) {
                        bool cocok = true;
                        for (size_t idx = 0; idx < pBlok.size(); idx++) {
                            int c1 = (tesKey[0][0] * pBlok[idx].first + tesKey[0][1] * pBlok[idx].second) % 26;
                            int c2 = (tesKey[1][0] * pBlok[idx].first + tesKey[1][1] * pBlok[idx].second) % 26;
                            if (c1 != cBlok[idx].first || c2 != cBlok[idx].second) {
                                cocok = false;
                                break;
                            }
                        }
                        if (cocok) {
                            for (int r = 0; r < 2; r++)
                                for (int c = 0; c < 2; c++)
                                    key[r][c] = tesKey[r][c];
                            return true;
                        }
                    }
                }
            }
        }
    }

    cout << "Kunci tidak ditemukan.\n";
    return false;
}

void cetakMatriks(int mat[2][2]) {
    cout << "[" << mat[0][0] << " " << mat[0][1] << "]\n";
    cout << "[" << mat[1][0] << " " << mat[1][1] << "]\n";
}

//main
int main() {
    int menu;
    do {
        cout << "\n===== HILL CIPHER 2x2 =====\n";
        cout << "1. Enkripsi\n2. Dekripsi\n3. Cari Key\n4. Keluar\n";
        cout << "Pilih menu: ";
        cin >> menu;

        if (menu == 1 || menu == 2) {
            int key[2][2];
            inputKey(key);

            string teks;
            cout << (menu == 1 ? "\nMasukkan Plaintext: " : "\nMasukkan Ciphertext: ");
            cin >> teks;

            cout << "\nHasil: " << prosesHill(teks, key, menu == 2) << endl;

        } else if (menu == 3) {
            string pTeks, cTeks;
            cout << "\nMasukkan Plaintext : "; cin >> pTeks;
            cout << "Masukkan Ciphertext: "; cin >> cTeks;

            int key[2][2];
            if (cariKey(pTeks, cTeks, key)) {
                cout << "\nKey Ditemukan:\n";
                cetakMatriks(key);
            }

        } else if (menu == 4) {
            cout << "\nSelesai.\n";
        } else {
            cout << "\nPilihan salah.\n";
        }
    } while (menu != 4);

    return 0;
}