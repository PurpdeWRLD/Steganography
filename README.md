📘 Steganography Project
A simple Steganography application implemented in C. This project hides secret messages inside image files (like BMP/PNG) using Least Significant Bit (LSB) encoding. It allows users to securely embed text into images and later extract it, demonstrating how data can be concealed in digital media.

---

✨ Features

🔒 Hide secret messages inside image files

🔍 Extract hidden messages from stego-images

📂 Supports common image formats (e.g., BMP/PNG depending on implementation)

💾 File handling for reading/writing binary image data

🧩 Demonstrates basic cryptography + information security concepts
 
 ---

🧠 Code Overview

main.c → Main program implementing encoding logic

encode.h & decode.h → Header file for handling image structures

encode.c → Helper functions for bit manipulation in encoding

decode.c → Helper functions for bit manipulation in decoding

beautigul.bmp → Original cover image

output.bmp → Image with hidden data

---

⚠️ Limitations

* Works only on uncompressed image formats (BMP)

* Message size limited to image capacity (1 bit per pixel channel)

*Not the actual encryption (message is hidden but not encrypted)

📄 License
This project is licensed under the MIT License.

👤 Author
Jaya Karunesh
