/* An actor that displays a AVI, Quicktime or MPEG video file.
 * 
 * Copyright (c) 2003-2006 The Regents of the University of California.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
package drone.jmf.actor;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.io.IOException;

import javax.media.ControllerEvent;
import javax.media.ControllerListener;
import javax.media.Manager;
import javax.media.MediaException;
import javax.media.Player;
import javax.media.Time;
import javax.media.protocol.DataSource;
import javax.swing.JFrame;

import ptolemy.actor.lib.Sink;
import ptolemy.data.ObjectToken;
import ptolemy.data.Token;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;

//////////////////////////////////////////////////////////////////////////
//// VideoPlayer

/** An actor that displays a AVI, Quicktime or MPEG video file.

 <p>This actor accepts an ObjectToken that contains a DataSource.  This
 is typically obtained from the output of the {@link StreamLoader}
 actor.  After the model is run, a window will pop up allowing control
 of playing, rate of playback, and volume control.

 @author James Yeh
 @version $Id: VideoPlayer.java,v 1.26 2006/08/21 23:11:51 cxh Exp $
 @since Ptolemy II 4.0
 @Pt.ProposedRating Red (cxh)
 @Pt.AcceptedRating Red (cxh)
 @see StreamLoader
 */
public class VideoPlayer extends Sink implements ControllerListener {
    /** Construct an actor with the given container and name.
     *  @param container The container.
     *  @param name The name of this actor.
     *  @exception IllegalActionException If the actor cannot be contained
     *   by the proposed container.
     *  @exception NameDuplicationException If the container already has an
     *   actor with this name.
     */
    public VideoPlayer(CompositeEntity container, String name)
            throws IllegalActionException, NameDuplicationException {
        super(container, name);

        input.setTypeEquals(BaseType.OBJECT);
    }

    /** React to notification of a change in controller status.
     *  @param event The event.
     */
    public synchronized void controllerUpdate(ControllerEvent event) {
        notifyAll();
    }

    /** Accept an ObjectToken containing a DataSource, and set it up
     *  for playing.
     *  @exception IllegalActionException If there is no director,
     *  if the file cannot be opened, or if the Java Media Framework
     *  throws an exception.
     *  @return super.postfire()
     */
    public boolean postfire() throws IllegalActionException {
        ObjectToken objectToken;
        Token token = input.get(0);

        try {
            objectToken = (ObjectToken) token;
        } catch (ClassCastException ex) {
            throw new IllegalActionException(this, ex, "Failed to cast "
                    + token.getClass() + " to an ObjectToken.\n"
                    + "The VideoPlayer actor expects to be connected to "
                    + "actors like the StreamLoader.\n"
                    + "Try connecting other actors to "
                    + "actor.lib.image.ImageDisplay.");
        }

        DataSource input = (DataSource) objectToken.getValue();

        if (_player != null) {
            _player.removeControllerListener(this);
        }

        try {
            _player = Manager.createRealizedPlayer(input);
            _player.addControllerListener(this);
            _player.prefetch();
        } catch (IOException ex) {
            throw new IllegalActionException(this, ex, "Cannot open file: "
                    + input);
        } catch (MediaException ex) {
            throw new IllegalActionException(this, ex,
                    "Exception thrown by media framework on " + input);
        }

        _player.setMediaTime(_startTime);

        _frame = new JFrame();
        _container = _frame.getContentPane();
        _container.setLayout(new BorderLayout());

        Component controlPanel = _player.getControlPanelComponent();
        Component videoPanel = _player.getVisualComponent();
        _container.add(videoPanel, BorderLayout.CENTER);
        _container.add(controlPanel, BorderLayout.SOUTH);
        _container.validate();
        _frame.pack();
        _frame.setVisible(true);

        _player.start();
        return super.postfire();
    }

    /** The container that contains the control panel components. */
    private Container _container;

    /** The JFrame where the the container is put. */
    private JFrame _frame;

    /** The player. */
    private Player _player;

    /** Start time for the video clip. */
    private Time _startTime = new Time(0.0);
}
