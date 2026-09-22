#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// untuk mengenkripsi Plaintext menjadi Ciphertext
string encryptVigenere(string pt, string key) {
    string ct = "";
    int keyLength = key.length();
    int keyIndex = 0;

    for (int i = 0; i < pt.length(); i++) {
        char c = pt[i];

        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            int nilaiPt = c - base;
            
            // Ambil karakter kunci yang sesuai dan konversi ke angka
            char hurufKunci = key[keyIndex % keyLength];
            int nilaiKey = toupper(hurufKunci) - 'A';

            // enkripsi
            int nilaiEnkripsi = (nilaiPt + nilaiKey) % 26;
            
            ct += (base + nilaiEnkripsi);
            keyIndex++; // Key bergeser pada karakter alfabet
        } else {
            ct += c;
        }
    }
    return ct;
}

// untuk mendekripsi Ciphertext menjadi Plaintext
string decryptVigenere(string ct, string key) {
    string pt = "";
    int keyLength = key.length();
    int keyIndex = 0;

    for (int i = 0; i < ct.length(); i++) {
        char c = ct[i];

        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            int nilaiCt = c - base;
            
            char hurufKunci = key[keyIndex % keyLength];
            int nilaiKey = toupper(hurufKunci) - 'A';

            // dekripsi
            int nilaiDekripsi = (nilaiCt - nilaiKey + 26) % 26; 
            
            pt += (base + nilaiDekripsi);
            keyIndex++;
        } else {
            pt += c;
        }
    }
    return pt;
}

int main() {
    int pilihan;
    string text, key;

    cout << "=== PROGRAM VIGENERE CIPHER ===\n";
    cout << "1. Enkripsi\n";
    cout << "2. Dekripsi\n";
    cout << "Pilih menu (1/2): ";
    cin >> pilihan;
    cin.ignore();

    if (pilihan == 1) {
        cout << "Masukkan Plaintext : ";
        getline(cin, text);
        cout << "Masukkan Key       : ";
        getline(cin, key);

        string hasil = encryptVigenere(text, key);
        cout << "\nHasil Enkripsi : " << hasil << endl;

    } else if (pilihan == 2) {
        cout << "Masukkan Ciphertext: ";
        getline(cin, text);
        cout << "Masukkan Key       : ";
        getline(cin, key);

        string hasil = decryptVigenere(text, key);
        cout << "\nHasil Dekripsi : " << hasil << endl;

    } else {
        cout << "Pilihan tidak valid!" << endl;
    }

    return 0;
}