import static java.lang.Math.cos;
import static java.lang.Math.sin;
import static java.lang.Math.sqrt;

import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.awt.image.IndexColorModel;
import java.awt.image.WritableRaster;

public class PlasmaCalculations {
	int box_w, box_h;
	private BufferedImage image;
	private short[] tab1;
	private short[] tab2;
	private IndexColorModel palette;

	public PlasmaCalculations(int width, int height) {
		super();
		this.box_w = width;
		this.box_h = height;
		init();
	}

	private void init() {
		System.out.println("pre calculate");

		this.image = new BufferedImage(box_w, box_h,
				BufferedImage.TYPE_BYTE_INDEXED);

		tab1 = new short[4 * box_w * box_h];
		CalcTab1();
		tab2 = new short[4 * box_w * box_h];
		CalcTab2();
		
		palette = ColorPalette.genPalette(10);
	}

	private BufferedImage genFrameImage(long millis) {
		// palette = ColorPalette.genPalette((int) (millis / 100d));
		return new BufferedImage(
				palette,
				image.getRaster(), false, null);
	}

	public BufferedImage genFrame(long millis) {
		BufferedImage frame = genFrameImage(millis);
		WritableRaster raster = frame.getRaster();
		DataBufferByte dataBuffer = (DataBufferByte) raster.getDataBuffer();
		byte[] data = dataBuffer.getData(0);
		updateData(data, millis);
		return frame;
	}

	private void updateData(byte[] data, double millis) {
		double tick = (millis / 10000.0);

		double circle1 = tick * 0.085 / 6d;
		double circle2 = -tick * 0.1 / 6d;
		double circle3 = tick * .3 / 6d;
		double circle4 = -tick * .2 / 6d;
		double circle5 = tick * .4 / 6d;
		double circle6 = -tick * .15 / 6d;
		double circle7 = tick * .35 / 6d;
		double circle8 = -tick * .05 / 6d;

		double roll = tick * 5;
		double h = box_h;
		double w = box_w;
		int x2 = (int) ((w / 2d) + (w / 2d) * sin(circle1));
		int y2 = (int) ((h / 2d) + (h / 2d) * cos(circle2));
		int x1 = (int) ((w / 2d) + (w / 2d) * cos(circle3));
		int y1 = (int) ((h / 2d) + (h / 2d) * sin(circle4));
		int x3 = (int) ((w / 2d) + (w / 2d) * cos(circle5));
		int y3 = (int) ((h / 2d) + (h / 2d) * sin(circle6));
		int x4 = (int) ((w / 2d) + (w / 2d) * cos(circle7));
		int y4 = (int) ((h / 2d) + (h / 2d) * sin(circle8));

		CalculateBody(data, x1, y1, x2, y2, x3, y3, x4, y4, (int) roll);
	}

	void CalculateBody(byte[] body, int x1, int y1, int x2, int y2, int x3,
			int y3, int x4, int y4, int roll) {

		System.out.println(x1 + "," + y1 + " " + x2 + "," + y2 + " " + x3 + ","
				+ y3 + " - " + roll);

		for (int i = 0; i < box_h; i++) {
			int k = i * box_w;
			for (int j = 0; j < box_w; j++) {
				// this is the heart of the plasma
				body[k + j] = (byte) (//
				tab1[box_w * (i + y1) + j + x1] + roll
						+ tab2[box_w * (i + y2) + j + x2]
						+ tab2[box_w * (i + y3) + j + x3] + //
				tab2[box_w * (i + y4) + j + x4]);
			}
		}
	}

	double metafactor = 0.5;

	void CalcTab1() // calculate table 1 for plasma
	{
		int i = 0, j = 0;
		double factor = metafactor * 5d * 520d / (double) (box_w + box_h);
		while (i < box_h * 2) {
			j = 0;
			while (j < box_w * 2) {
				tab1[(i * box_w * 2) + j] = (byte) ((sqrt(16.0 + (box_h - i)
						* (box_h - i) + (box_w - j) * (box_w - j)) - 4) * factor);
				j++;
			}
			i++;
		}
	}

	void CalcTab2() // calculate table 2 for plasma
	{
		int i = 0, j = 0;
		double temp;
		double factor = metafactor * 520d / (double) (box_w + box_h);
		while (i < box_h * 2) {
			j = 0;
			while (j < box_w * 2) {
				temp = (sqrt(16.0 + (box_h - i) * (box_h - i) + (box_w - j)
						* (box_w - j)) - 4)
						* factor;
				tab2[(i * box_w * 2) + j] = (byte) ((sin(temp / 9.5) + 1) * 90);
				// tab2[(i*box_w*2)+j]=(sin(sqrt((box_h-i)*(box_h-i)+(box_w-j)*(box_w-j))/9.5)+1)*90;
				j++;
			}
			i++;
		}
	}
}
