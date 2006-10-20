package drone.core;

import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Constructor;
import java.net.URI;
import java.net.URL;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import org.java.plugin.boot.Application;
import org.java.plugin.boot.Boot;
import org.java.plugin.registry.Extension;
import org.java.plugin.registry.ExtensionPoint;

import drone.core.extensions.FailToCreateComponentException;
import drone.core.extensions.ViewExtension;

import ptolemy.actor.CompositeActor;
import ptolemy.actor.Director;
import ptolemy.actor.ExecutionListener;
import ptolemy.actor.Manager;
import ptolemy.actor.gui.Configuration;
import ptolemy.actor.gui.Effigy;
import ptolemy.actor.gui.ModelDirectory;
import ptolemy.actor.gui.PtolemyEffigy;
import ptolemy.data.expr.Parameter;
import ptolemy.data.expr.StringParameter;
import ptolemy.kernel.ComponentEntity;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.attributes.VersionAttribute;
import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.InternalErrorException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.kernel.util.NamedObj;
import ptolemy.kernel.util.Settable;
import ptolemy.kernel.util.Workspace;
import ptolemy.moml.Documentation;
import ptolemy.moml.MoMLChangeRequest;
import ptolemy.moml.MoMLParser;
import ptolemy.moml.filter.BackwardCompatibility;
import ptolemy.util.JNLPUtilities;
import ptolemy.util.MessageHandler;
import ptolemy.util.StringUtilities;
import ptolemy.vergil.VergilApplication;

import drone.core.extensions.*;

public class CoreApplication extends VergilApplication implements Application, ExecutionListener {

	/**
	 * Parse the specified command-line arguments, instanting classes and
	 * reading files that are specified.
	 * 
	 * @param basePath
	 *            The basePath to look for configurations in, usually
	 *            "ptolemy/configs", but other tools might have other
	 *            configurations in other directories
	 * @param args
	 *            The command-line arguments.
	 * @exception Exception
	 *                If command line arguments have problems.
	 */
	public CoreApplication(String basePath, String[] args, CorePlugin corePlugin) throws Exception {
		super(basePath, args);
//		_corePlugin = corePlugin;
//
//		// The Java look & feel is pretty lame, so we use the native
//		// look and feel of the platform we are running on.
//		// NOTE: This creates the only dependence on Swing in this
//		// class. Should this be left to derived classes?
//		try {
//			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
//		} catch (Exception e) {
//			// Ignore exceptions, which only result in the wrong look and feel.
//		}
//
//		// Create a parser to use.
//		_parser = new MoMLParser();
//
//		// We set the list of MoMLFilters to handle Backward Compatibility.
//		MoMLParser.setMoMLFilters(BackwardCompatibility.allFilters());
//
//		// 2/03: Moved the setMessageHandler() to before parseArgs() so
//		// that if we get an error in parseArgs() we will get a graphical
//		// stack trace. Such an error could be caused by specifying a model
//		// as a command line argument and the model has an invalid parameter.
//		// MessageHandler.setMessageHandler(new GraphicalMessageHandler());
//
//		// Even if the user is set up for foreign locale, use the US locale.
//		// This is because certain parts of Ptolemy (like the expression
//		// language) are not localized.
//		// FIXME: This is a workaround for the locale problem, not a fix.
//		// FIXME: In March, 2001, Johan Ecker writes
//		// Ptolemy gave tons of exception when started on my laptop
//		// which has Swedish settings as default. The Swedish standard
//		// for floating points are "2,3", i.e. using a comma as
//		// delimiter. However, I think most Swedes are adaptable and
//		// do not mind using a dot instead since this more or less has
//		// become the world standard, at least in engineering. The
//		// problem is that I needed to change my global settings to
//		// start Ptolemy and this is quite annoying. I guess that the
//		// expression parser should just ignore the delimiter settings
//		// on the local computer and always use dot, otherwise Ptolemy
//		// will crash using its own init files.
//		try {
//			java.util.Locale.setDefault(java.util.Locale.US);
//		} catch (java.security.AccessControlException accessControl) {
//			// FIXME: If the application is run under Web Start, then this
//			// exception will be thrown.
//		}
//
//		try {
//			_parseArgs(args);
//
//			// Run if -run argument was specified.
//			if (_run) {
//				runModels();
//
//				if (_exit) {
//					// In vergil, this gets called in the
//					// swing thread, which hangs everything
//					// if we call waitForFinish() directly.
//					// So instead, we create a new thread to
//					// do it.
//					Thread waitThread = new Thread() {
//						public void run() {
//							waitForFinish();
//							StringUtilities.exit(0);
//						}
//					};
//
//					// Note that we start the thread here, which could
//					// be risky when we subclass, since the thread will be
//					// started before the subclass constructor finishes
//					// (FindBugs)
//					waitThread.start();
//				}
//			}
//		} catch (Throwable ex) {
//			// Make sure that we do not eat the exception if there are
//			// problems parsing. For example, "ptolemy -FOO bar bif.xml"
//			// will crash if bar is not a variable. Instead, use
//			// "ptolemy -FOO \"bar\" bif.xml"
//			// Accumulate the arguments into a StringBuffer
//			StringBuffer argsStringBuffer = new StringBuffer();
//
//			try {
//				for (int i = 0; i < args.length; i++) {
//					if (argsStringBuffer.length() > 0) {
//						argsStringBuffer.append(" ");
//					}
//
//					argsStringBuffer.append(args[i]);
//				}
//			} catch (Exception ex2) {
//				// Ignored
//			}
//
//			// Make sure we throw an exception if one is caught.
//			// If we don't, then running vergil -foo will just exit.
//			throw new Exception("Failed to parse \""
//					+ argsStringBuffer.toString() + "\"", ex);
//		}
	}

	public void startApplication() throws Exception {
		// Docking windwos should be run in the Swing thread
//		SwingUtilities.invokeLater(new Runnable() {
//			public void run() {
//				createMainWindow();
//				parseEntityLibraryExtensions();
//				parseViewExtensions();
//			}
//		});
	}

//	protected void createMainWindow() {
//		try {
//		_mainWindow = new MainWindow(); 
//		_mainWindow.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
//		_mainWindow.addWindowListener(new WindowAdapter() {
//			public void windowClosed(final WindowEvent e) {
//				try {
//					JOptionPane.getRootFrame().dispose();
//					Boot.stopApplication(CoreApplication.this);
//				} catch (Exception ex) {
//					ex.printStackTrace();
//				}
//				System.exit(0);
//			}
//		});
//		} catch(Exception ex) {
//			ex.printStackTrace();
//		}
//	}
	
//	protected void parseEntityLibraryExtensions() {
//		// TODO cleanup this code a bit, lots of things in common with parseViewExtensions
//		ExtensionPoint extensionPoint = _corePlugin.getManager().getRegistry()
//				.getExtensionPoint(_corePlugin.getDescriptor().getId(), "entity-library");
//		for (Iterator it = extensionPoint.getConnectedExtensions().iterator(); it
//				.hasNext();) {
//			Extension ext = (Extension) it.next();
//            try {
//                // Activate plug-in that declares extension.
//            	_corePlugin.getManager().activatePlugin(
//                        ext.getDeclaringPluginDescriptor().getId());
//            	URL fileURL = _corePlugin.getManager().getPluginClassLoader(ext.getDeclaringPluginDescriptor())
//            		.getResource(ext.getParameter("file").valueAsString());
//            	// TODO: do we need this?
//                String label = ext.getParameter("label").valueAsString();
//                String description = ext.getParameter("description").valueAsString();
//                
//        		Configuration configuration = (Configuration) Configuration.configurations().iterator().next();
//        		if (configuration == null) {
//        			throw new IllegalActionException("There are no existing configurations.");
//        		}
//
//        		MoMLParser parser = new MoMLParser(configuration.workspace());
//        		parser.reset();
//
//        		CompositeEntity library = (CompositeEntity) parser.parse(fileURL, fileURL);
//        		// FIXME: je suis rendu la...
//        		library.setContainer((CompositeEntity)configuration.getEntity("actor library"));
//            } catch (Throwable t) {
//            	//TODO: log4net
//            	System.out.println(t.getMessage());
//            }
//		}
//		//Add all dockedExtensions to the mainWindow
//		//TODO: to be implemented in mainwindow
//	}
//
//
//	protected void parseViewExtensions() {
//		ExtensionPoint dockedExtPoint = _corePlugin.getManager().getRegistry()
//				.getExtensionPoint(_corePlugin.getDescriptor().getId(), "view");
//		
//		for (Iterator it = dockedExtPoint.getConnectedExtensions().iterator(); it
//				.hasNext();) {
//			Extension ext = (Extension) it.next();
//            try {
//                // Activate plug-in that declares extension.
//            	_corePlugin.getManager().activatePlugin(
//                        ext.getDeclaringPluginDescriptor().getId());
//                // Get plug-in class loader.
//                ClassLoader classLoader = _corePlugin.getManager().getPluginClassLoader(
//                        ext.getDeclaringPluginDescriptor());
//                // Load class.
//                Class dockedExtCls = classLoader.loadClass(
//                        ext.getParameter("class").valueAsString());
//                // Create instance.
//                String label = ext.getParameter("label").valueAsString();
//                // TODO: change addDockedExtension to addViewExtension
//        		_mainWindow.addDockedExtension(label, (ViewExtension) dockedExtCls.newInstance());
//
//            } catch (Throwable t) {
//            	//TODO: log4net
//            	System.out.println(t.getMessage());
//            }
//		}
//		//Add all dockedExtensions to the mainWindow
//		//TODO: to be implemented in mainwindow
//	}
//	
//	MainWindow _mainWindow;

}
