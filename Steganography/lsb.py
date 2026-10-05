from PIL import Image

def text_to_bin(text):
    """Mengubah teks string menjadi string biner 8-bit per karakter."""
    return ''.join(format(ord(char), '08b') for char in text)

def bin_to_text(binary_data):
    """Mengubah string biner kembali menjadi string teks."""
    all_bytes = [binary_data[i: i+8] for i in range(0, len(binary_data), 8)]
    decoded_text = ""
    for byte in all_bytes:
        try:
            decoded_text += chr(int(byte, 2))
        except ValueError:
            break
    return decoded_text

def encode_image(image_path, secret_message, output_path):
    """Menyembunyikan pesan rahasia ke dalam citra menggunakan metode LSB."""
    try:
        image = Image.open(image_path)
    except FileNotFoundError:
        print(f"[ERROR] File gambar '{image_path}' tidak ditemukan!")
        return
        
    img_rgb = image.convert('RGB')
    pixels = img_rgb.load()
    width, height = img_rgb.size
    
    # Tambahkan delimiter penanda akhir pesan
    secret_message += "###"
    binary_message = text_to_bin(secret_message)
    
    max_bits = width * height * 3
    if len(binary_message) > max_bits:
        print("[ERROR] Pesan terlalu panjang untuk disembunyikan pada gambar ini!")
        return
    
    data_index = 0
    
    for y in range(height):
        for x in range(width):
            if data_index >= len(binary_message):
                break
            
            r, g, b = pixels[x, y]
            
            if data_index < len(binary_message):
                r = (r & ~1) | int(binary_message[data_index])
                data_index += 1
                
            if data_index < len(binary_message):
                g = (g & ~1) | int(binary_message[data_index])
                data_index += 1
                
            if data_index < len(binary_message):
                b = (b & ~1) | int(binary_message[data_index])
                data_index += 1
                
            pixels[x, y] = (r, g, b)
            
        if data_index >= len(binary_message):
            break
            
    img_rgb.save(output_path)
    print(f"\n[SUKSES] Pesan berhasil disembunyikan!\nFile stego-image tersimpan sebagai: '{output_path}'")

def decode_image(image_path):
    """Mengekstrak pesan rahasia dari stego-image menggunakan LSB."""
    try:
        image = Image.open(image_path)
    except FileNotFoundError:
        print(f"[ERROR] File stego-image '{image_path}' tidak ditemukan!")
        return
        
    img_rgb = image.convert('RGB')
    pixels = img_rgb.load()
    width, height = img_rgb.size
    
    binary_data = ""
    for y in range(height):
        for x in range(width):
            r, g, b = pixels[x, y]
            binary_data += str(r & 1)
            binary_data += str(g & 1)
            binary_data += str(b & 1)
        
    all_bytes = [binary_data[i: i+8] for i in range(0, len(binary_data), 8)]
    decoded_message = ""
    for byte in all_bytes:
        try:
            char = chr(int(byte, 2))
            decoded_message += char
            if decoded_message.endswith("###"):
                print(f"\n[SUKSES] Pesan rahasia ditemukan!")
                print(f"-> Isi Pesan: {decoded_message[:-3]}")
                return
        except ValueError:
            break
            
    print("\n[INFO] Tidak ditemukan pesan rahasia atau format gambar tidak valid.")

# --- MENU UTAMA PROGRAM ---
if __name__ == "__main__":
    while True:
        print("\n========================================")
        print("         PROGRAM STEGANOGRAFI LSB         ")
        print("========================================")
        print("1. Encode (Sembunyikan Pesan ke Gambar)")
        print("2. Decode (Baca Pesan dari Gambar)")
        print("3. Keluar")
        
        pilihan = input("\nPilih menu (1/2/3): ").strip()
        
        if pilihan == "1":
            print("\n--- MENU ENCODE ---")
            img_in = input("Masukkan nama file cover ").strip()
            pesan = input("Masukkan pesan rahasia yang ingin disembunyikan: ").strip()
            img_out = input("Masukkan nama file hasil output: ").strip()
            encode_image(img_in, pesan, img_out)
            
        elif pilihan == "2":
            print("\n--- MENU DECODE ---")
            img_target = input("Masukkan nama file stego-image yang mau dibaca: ").strip()
            decode_image(img_target)
            
        elif pilihan == "3":
            print("\nKeluar dari program. Terima kasih!")
            break
        else:
            print("\nSilakan pilih angka 1, 2, atau 3.")