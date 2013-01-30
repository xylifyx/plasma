package jet.plasma;

import java.awt.Graphics;
import java.awt.image.BufferedImage;

@SuppressWarnings("serial")
public class PlasmaAnimation extends AnimationComponent {

	public PlasmaAnimation(int width, int height) {
		super(width, height);
		delay = 25;
	}
	PlasmaCalculations image;

	@Override
	protected void recalculate(int width, int height) {
		image = new PlasmaCalculations(width, height);
	}
	BufferedImage currentFrame;

	@Override
	protected void paintAnimationFrame(long millis) {
		currentFrame = image.genFrame(millis);
		//repaint();
		paintNow();
	}

	private void paintNow() {
		final Graphics g = getGraphics();
		paintNow(g);
		g.dispose();
	}
	long t = System.currentTimeMillis();
	int frameCount = 0;

	public void paintNow(Graphics g) {
		frameCount++;
		final long t2 = System.currentTimeMillis();
		if (t2 - t > 2000) {
			System.out.println("framerate: " + (frameCount * 1000 / (t2 - t)));
			t = t2;
			frameCount = 0;
		}
		g.drawImage(currentFrame, 0, 0, PlasmaAnimation.this);
	}

	public static void main(String[] args) {
		PlasmaAnimation animationComponent = new PlasmaAnimation(320, 200);
		animationComponent.showInFrame();
	}
}
