import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.lang.reflect.InvocationTargetException;

import javax.swing.SwingUtilities;

@SuppressWarnings("serial")
public class PlasmaAnimation extends AnimationComponent {

	public PlasmaAnimation(int width, int height) {
		super(width, height);
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
		repaint();
					
	}
	
	public void paint(Graphics g) {
		g.drawImage(currentFrame, 0, 0, PlasmaAnimation.this);
	}

	public static void main(String[] args) {
		PlasmaAnimation animationComponent = new PlasmaAnimation(320, 200);
		animationComponent.showInFrame();
	}
}
