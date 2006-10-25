package drone.core;

import java.awt.BorderLayout;
import java.awt.MenuBar;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.MouseEvent;
import java.net.URL;
import java.util.List;
import java.util.ListIterator;

import javax.swing.Icon;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JScrollPane;
import javax.swing.JToolBar;
import javax.swing.JTree;

import net.infonode.docking.DockingWindow;
import net.infonode.docking.DockingWindowListener;
import net.infonode.docking.OperationAbortedException;
import net.infonode.docking.RootWindow;
import net.infonode.docking.SplitWindow;
import net.infonode.docking.TabWindow;
import net.infonode.docking.View;
import net.infonode.docking.ViewSerializer;
import net.infonode.docking.WindowBar;
import net.infonode.docking.drag.DockingWindowDragSource;
import net.infonode.docking.drag.DockingWindowDragger;
import net.infonode.docking.drag.DockingWindowDraggerProvider;
import net.infonode.docking.properties.RootWindowProperties;
import net.infonode.docking.theme.DockingWindowsTheme;
import net.infonode.docking.theme.ShapedGradientDockingTheme;
import net.infonode.docking.util.DockingUtil;
import net.infonode.docking.util.StringViewMap;
import net.infonode.docking.util.ViewMap;
import net.infonode.util.Direction;

import ptolemy.actor.gui.Tableau;
import ptolemy.gui.Top;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.Entity;
import ptolemy.moml.MoMLParser;
import ptolemy.vergil.basic.BasicGraphFrame;
import ptolemy.vergil.tree.PTree;
import ptolemy.vergil.tree.VisibleTreeModel;

import drone.core.extensions.ViewExtension;
import drone.core.extensions.FailToCreateComponentException;

public class MainWindow extends JFrame implements DockingWindowListener {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public static final float DEFAULT_LEFT_PROPORTION = 0.25f;
	public static final float DEFAULT_UPPER_LEFT_PROPORTION = 0.5f;
	public static final float DEFAULT_UPPER_RIGHT_PROPORTION = 0.75f;
	
	private MainWindow() throws Exception {
		createMenus();
		createRootWindow();
		setDefaultLayout();
		createDefaultViews();
		createToolBar();
		showFrame();
	}

	/**
	 * Creates the root window and the views.
	 */
	private void createRootWindow() {
		_viewMap = new StringViewMap();
		_upperLeftTabWindow = new TabWindow();
		_bottomLeftTabWindow = new TabWindow();
		_upperRightTabWindow = new TabWindow();
		_bottomRightTabWindow = new TabWindow();
		_rootWindow = new RootWindow(_viewMap);
		
		// Set gradient theme. The theme properties object is the super object of our properties object, which
	    // means our property value settings will override the theme values
		DockingWindowsTheme shapedGradientDockingTheme = new ShapedGradientDockingTheme();	    
		_rootWindowProperties.addSuperObject(shapedGradientDockingTheme.getRootWindowProperties());

	    // Our properties object is the super object of the root window properties object, so all property values of the
	    // theme and in our property object will be used by the root window
	    _rootWindow.getRootWindowProperties().addSuperObject(_rootWindowProperties);

	    // Enable the bottom window bar
	    _rootWindow.getWindowBar(Direction.DOWN).setEnabled(true);
	}
	
	public void createDefaultViews() {
		_libraryBrowser = new View("Library browser", null, _defaultLibraryBrowserComponent);
		//TODO: must have unique naming for window view name, for serialization
		_viewMap.addView("Library browser", _libraryBrowser);
		_upperLeftTabWindow.addTab(_libraryBrowser);
	}
	
	
	protected void createToolBar() {
		if (_defaultToolBar == null) {
			_defaultToolBar = new JToolBar();
			_defaultToolBar.add(new JLabel("No toolbar available in this view."));
		}
		_toolBar = _defaultToolBar;
		getContentPane().add(_toolBar, BorderLayout.NORTH);
	}

	public void addDockedExtension(String label, ViewExtension extension) {
		ViewExtension.Position position = extension.position();
		View view = null;
		try {
			view = new View(label, null, extension.createComponent());
		} catch (FailToCreateComponentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		//TODO: must have unique naming for window view name, for serialization
		_viewMap.addView(label, view);
		if (position == ViewExtension.Position.UPPER_LEFT) {
			_upperLeftTabWindow.addTab(view);
		} else if (position == ViewExtension.Position.BOTTOM_LEFT) {
			_bottomLeftTabWindow.addTab(view);
		} else if (position == ViewExtension.Position.UPPER_RIGHT) {
			_upperRightTabWindow.addTab(view);
		} else {
			_bottomRightTabWindow.addTab(view);
		}
	}
	
	public void showTableau(Tableau tableau, ViewExtension.Position position) {
		View view = null;
		JFrame tableauFrame = tableau.getFrame();
		if (tableauFrame instanceof Top) {
			((Top)tableauFrame).setCentering(false);
		}
		tableauFrame.pack();
		view = new TableauView(null, tableau);
		view.addListener(this);
		//TODO: must have unique naming for window view name, for serialization
		_viewMap.addView(tableau.getTitle(), view);
		if (position == ViewExtension.Position.UPPER_LEFT) {
			_upperLeftTabWindow.addTab(view);
		} else if (position == ViewExtension.Position.BOTTOM_LEFT) {
			_bottomLeftTabWindow.addTab(view);
		} else if (position == ViewExtension.Position.UPPER_RIGHT) {
			_upperRightTabWindow.addTab(view);
		} else {
			_bottomRightTabWindow.addTab(view);
		}
		_focusTableau(tableau);
	}
	
	/**
	 * Sets the default window layout.
	 * @throws Exception 
	 */
	private void setDefaultLayout() throws Exception {
		
//		_upperLeftTabWindow.getChildWindowCount();
//		i = extensionList.iterator();
//		
//		for (; it.hasNext(); it.next())
		_rootWindow.setWindow(
				new SplitWindow(true,
								DEFAULT_LEFT_PROPORTION,
								new SplitWindow(false,
												DEFAULT_UPPER_LEFT_PROPORTION,
												_upperLeftTabWindow,
												_bottomLeftTabWindow),
								new SplitWindow(false,
												DEFAULT_UPPER_RIGHT_PROPORTION,
												_upperRightTabWindow,
												_bottomRightTabWindow))
		);

	}

	/**
	 * Initializes the frame and shows it.
	 */
	private void showFrame() {
//		frame.getContentPane().add(createToolBar(), BorderLayout.NORTH);
		getContentPane().add(_rootWindow, BorderLayout.CENTER);
		//setJMenuBar(createMenuBar());
		setSize(900, 700);
		setVisible(true);
	}

	private void createMenus() {
		// Creates dummy menus.
		// This ensures that the menu is created first, thus avoiding buggy behavior upon
		// calling _focusTableau().
		JMenuBar menuBar = new JMenuBar();
		menuBar.add(new JMenu("Dummy"));
		menuBar.setVisible(true);
		setJMenuBar(menuBar);
	}
	
	private RootWindow _rootWindow;
	private StringViewMap _viewMap;
	private TabWindow _upperLeftTabWindow;
	private TabWindow _bottomLeftTabWindow;
	private TabWindow _upperRightTabWindow;
	private TabWindow _bottomRightTabWindow;
	  /**
	   * In this properties object the modified property values for close buttons etc. are stored. This object is cleared
	   * when the theme is changed.
	   */
	private RootWindowProperties _rootWindowProperties = new RootWindowProperties();

	
//	protected void buildMenu() {
//	// Create file menu.
//	QMenu fileMenu = menuBar().addMenu(tr("File"));

//	QAction newAct = new QAction(tr("&New"), this);
//	newAct.setShortcut(tr("Ctrl+N"));
//	newAct.setStatusTip(tr("Create a new file"));
//	fileMenu.addAction(newAct);

//	QAction openAct = new QAction(tr("&Open..."), this);
//	openAct.setShortcut(tr("Ctrl+O"));
//	openAct.setStatusTip(tr("Open an existing file"));
//	fileMenu.addAction(openAct);

//	// Create edit menu.
//	QMenu editMenu = menuBar().addMenu(tr("Edit"));
//	}
	public static MainWindow instance() { 
		if (_instance == null) {
			try {
				_instance = new MainWindow();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}	
		return _instance; 
	}

	private void _focusTableau(Tableau tableau) {
		// Change menu bar.
		JFrame frame = tableau.getFrame();
		JMenuBar menuBar = frame.getJMenuBar();
		menuBar.setVisible(true);
		setJMenuBar(menuBar);
		
		if (frame instanceof Top)
			((Top)frame).hideMenuBar();
		
		if (frame instanceof BasicGraphFrame) {
			// Check if we have a library tree associated.
			_libraryBrowser.setComponent(new JScrollPane(((BasicGraphFrame)frame).getLibrary()));
			// Check if we have a toolbar associated.
			getContentPane().remove(_toolBar);
			_toolBar = ((BasicGraphFrame)frame).getJToolbar();
		} else {
			_libraryBrowser.setComponent(_defaultLibraryBrowserComponent);
			getContentPane().remove(_toolBar);
			_toolBar = _defaultToolBar;
		}
		getContentPane().add(_toolBar, BorderLayout.NORTH);
		
	}
	
	private static MainWindow _instance = null;

	public void viewFocusChanged(View previouslyFocusedView, View focusedView) {
		// TODO Auto-generated method stub
		if (focusedView instanceof TableauView) {
			_focusTableau(((TableauView)focusedView).getTableau());
		}
	}

	public void windowAdded(DockingWindow addedToWindow, DockingWindow addedWindow) {
		// TODO Auto-generated method stub
		
	}

	public void windowClosed(DockingWindow window) {
		// TODO Auto-generated method stub
		
	}

	public void windowClosing(DockingWindow window) throws OperationAbortedException {
		// TODO Auto-generated method stub
		
	}

	public void windowHidden(DockingWindow window) {
		// TODO Auto-generated method stub
		
	}

	public void windowRemoved(DockingWindow removedFromWindow, DockingWindow removedWindow) {
		// TODO Auto-generated method stub
		
	}

	public void windowShown(DockingWindow window) {
		// TODO Auto-generated method stub
		
	}

	private View _libraryBrowser;
	private static JComponent _defaultLibraryBrowserComponent = new JLabel("Library not available for this view.");
	
	private JToolBar _toolBar;
	private static JToolBar _defaultToolBar = null;
}

