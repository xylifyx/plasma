import static java.lang.Math.PI;
import static java.lang.Math.cos;

import java.awt.image.IndexColorModel;

public class ColorPalette {
	private final static double pi = PI;

	private static byte red[] = new byte[256];
	private static byte green[] = new byte[256];
	private static byte blue[] = new byte[256];

	private static void SetPal(int i, int r, int g, int b) {
		red[i] = (byte) r;
		green[i] = (byte) g;
		blue[i] = (byte) b;
	}

	private static int mycol(double u, double a) {
		return (int) ((cos((u) + (a)) + 1) * 31);
	}

	public static IndexColorModel genPalette(int tick) {
		CalculateColors(tick);

		IndexColorModel colorModel = new IndexColorModel(8, 256, //
				red, green, blue);
		return colorModel;
	}

	private static void CalculateColors(int tick) {
		double r = 1.0 / 6.0 * pi + tick * 0.05;
		double g = 3.0 / 6.0 * pi + tick * 0.05;
		double b = 5.0 / 6.0 * pi + tick * 0.1;

		for (int i = 0; i < 256; i++) {
			double u = 2 * pi / 256 * i;
			// #define mycol(u,a) (max(0.0,cos((u)+(a))))*63 // try this line
			// instead
			// #define mycol(u,a) (cos((u)+(a))+1)*31
			SetPal(i, mycol(u, r), mycol(u, g), mycol(u, b));
		}
	}

}
