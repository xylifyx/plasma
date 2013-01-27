import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.SwingWorker;

@SuppressWarnings("serial")
public class AnimationComponent extends JComponent {
	protected int w;
	protected int h;
	private long dt;

	public AnimationComponent(int width, int height) {
		this.w = width;
		this.h = height;
		setPreferredSize(new Dimension(w, h));
		addComponentListener(new ComponentAdapter() {

			@Override
			public void componentResized(ComponentEvent e) {
				resize();
			}

		});
		this.dt = System.currentTimeMillis();
	}

	@Override
	public void paint(Graphics g) {

	}

	private void resize() {
		if (getWidth() != w || getHeight() != h) {
			w = getWidth();
			h = getHeight();
			recalculateInBackground();
		}
	}

	private void recalculateInBackground() {
		stop();
		new SwingWorker<Void, Void>() {

			@Override
			protected Void doInBackground() throws Exception {
				recalculate(w, h);
				return null;
			}

			public void done() {
				if (isVisible())
					start();
			}

		}.execute();
	}

	protected void recalculate(int width, int height) {

	}

	private void nextAnimationFrame() {
		paintAnimationFrame(System.currentTimeMillis() - dt);
	}

	protected void paintAnimationFrame(long millis) {

	}

	private static Timer timer = new Timer();
	TimerTask task;

	private void start() {
		if (task != null)
			stop();
		task = new TimerTask() {

			@Override
			public void run() {
				nextAnimationFrame();
			}
		};
		timer.schedule(task, 100, 100);
	}

	private void stop() {
		if (task != null)
			task.cancel();
		task = null;
	}

	@Override
	public void addNotify() {
		super.addNotify();
		recalculateInBackground();
	}

	@Override
	public void removeNotify() {
		super.removeNotify();
		stop();
	}

	// public static void main(String[] args) {
	// AnimationComponent animationComponent = new AnimationComponent(500, 500);
	// animationComponent.showInFrame();
	// }

	protected void showInFrame() {
		JFrame f = new JFrame("Plasma");
		f.getContentPane().add(this);
		f.pack();
		f.setLocationByPlatform(true);
		f.setVisible(true);
	}
}
