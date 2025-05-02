// png_printer.cpp
#include "./include/bmp_printer.hpp"


bool BMPPrinter::print(const uint32_t* argb_pixels, int width, int height, ScaleTarget scale_target, Alignment alignment) {

	return false; 
	/*
	std::string filepath = get_timestamped_filename();
	filepath = filepath + ".bmp";

	BMPHeader header;
	BMPInfoHeader info;

	info.width = width;
	info.height = height;

	int rowSize = (width * 3 + 3) & ~3; // jede Zeile auf 4-Byte ausrichten
	info.imageSize = rowSize * height;
	header.fileSize = header.dataOffset + info.imageSize;

	std::ofstream file(filename, std::ios::binary);
	if (!file) {
		std::cerr << "Fehler: Kann BMP-Datei nicht schreiben: " << filename << std::endl;
		return;
	}

	file.write(reinterpret_cast<const char*>(&header), sizeof(header));
	file.write(reinterpret_cast<const char*>(&info), sizeof(info));

	// BMP speichert von unten nach oben!
	for (int y = height - 1; y >= 0; --y) {
		for (unsigned int x = 0; x < width; ++x) {
			unsigned int pixel = full_buffer[y * width + x];

			// BMP erwartet BGR, nicht ARGB!
			byte blue = (pixel) & 0xFF;
			byte green = (pixel >> 8) & 0xFF;
			byte red = (pixel >> 16) & 0xFF;

			file.put(blue);
			file.put(green);
			file.put(red);
		}

		// Padding auf 4 Byte pro Zeile
		int rowSize = width * 3;
		int padding = (4 - (rowSize % 4)) % 4;
		for (int i = 0; i < padding; ++i) {
			file.put(0);
		}
	}


	file.close();
	*/
	
}





/*
	

	/*
		if (!stbi_write_png(filepath.c_str(), new_width, new_height, 3, scaled_data.data(), new_width * 3)) {
			display_message("Error writing PNG.");
		}
		else
			display_message("Printed to ; " + filepath);
			*/

//display_message("Printed to  " + filepath);
