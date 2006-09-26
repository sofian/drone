# Tests for the undoable feature of MoMLParser class
#
# @Author: Neil Smyth
#
# @Version: $Id: UndoLink.tcl,v 1.7 2005/02/28 20:19:39 cxh Exp $
#
# @Copyright (c) 1997-2005 The Regents of the University of California.
# All rights reserved.
#
# Permission is hereby granted, without written agreement and without
# license or royalty fees, to use, copy, modify, and distribute this
# software and its documentation for any purpose, provided that the
# above copyright notice and the following two paragraphs appear in all
# copies of this software.
#
# IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
# FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
# ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
# THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
# PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
# CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
# ENHANCEMENTS, OR MODIFICATIONS.
#
# 						PT_COPYRIGHT_VERSION_2
# 						COPYRIGHTENDKEY
#######################################################################

# Load the test definitions.
if {[string compare test [info procs test]] == 1} then {
    source testDefs.tcl
} {}

# Uncomment this to get a full report, or set in your Tcl shell window.
# set VERBOSE 1

# If a file contains non-graphical tests, then it should be named .tcl
# If a file contains graphical tests, then it should be called .itcl
#
# It would be nice if the tests would work in a vanilla itkwish binary.
# Check for necessary classes and adjust the auto_path accordingly.
#

######################################################################
####
#

# The XML header entry to use
set header {<?xml version="1.0" standalone="no"?>
<!DOCTYPE entity PUBLIC "-//UC Berkeley//DTD MoML 1//EN"
    "http://ptolemy.eecs.berkeley.edu/xml/dtd/MoML_1.dtd">}

######################################################################
####
#

# The base model to use for the entity tests
set entityTestModelBody {
  <entity name="top" class="ptolemy.actor.TypedCompositeActor">
     <entity name="level" class="ptolemy.actor.TypedCompositeActor">
        <port name="output" class="ptolemy.actor.TypedIOPort">
            <property name="output"/>
            <property name="multiport"/>
        </port>
        <entity name="a" class="ptolemy.actor.TypedCompositeActor">
              <port name="input" class="ptolemy.actor.TypedIOPort">
                <property name="input"/>
              </port>
              <port name="multi" class="ptolemy.actor.TypedIOPort">
                <property name="multiport"/>
              </port>
        </entity>
        <relation name="r1" class="ptolemy.actor.TypedIORelation" />
        <link port="a.input" relation="r1"/>
        <link port="output" relation="r1"/>
     </entity>
</entity>
}

set entityTestModel "$header $entityTestModelBody"


######################################################################
####
#

test UndoLink-1.1a {Test undoing a link: outside link, given relation, no index} {
    # Create a base model.
    set parser [java::new ptolemy.moml.MoMLParser]
    set toplevel [java::cast ptolemy.actor.CompositeActor [$parser parse $entityTestModel]]
    set manager [java::new ptolemy.actor.Manager [$toplevel workspace] "w"]
    $toplevel setManager $manager

    set change [java::new ptolemy.moml.MoMLChangeRequest $toplevel $toplevel {
        <entity name=".top.level">
            <link port="a.multi" relation="r1"/>
        </entity>
    }]
    # Mark the change as being undoable
    $change setUndoable true

    # NOTE: Request is filled immediately because the model is not running.
    $manager requestChange $change

    # Export the modified MoML
    $toplevel exportMoML
} {<?xml version="1.0" standalone="no"?>
<!DOCTYPE entity PUBLIC "-//UC Berkeley//DTD MoML 1//EN"
    "http://ptolemy.eecs.berkeley.edu/xml/dtd/MoML_1.dtd">
<entity name="top" class="ptolemy.actor.TypedCompositeActor">
    <entity name="level" class="ptolemy.actor.TypedCompositeActor">
        <port name="output" class="ptolemy.actor.TypedIOPort">
            <property name="output"/>
            <property name="multiport"/>
        </port>
        <entity name="a" class="ptolemy.actor.TypedCompositeActor">
            <port name="input" class="ptolemy.actor.TypedIOPort">
                <property name="input"/>
            </port>
            <port name="multi" class="ptolemy.actor.TypedIOPort">
                <property name="multiport"/>
            </port>
        </entity>
        <relation name="r1" class="ptolemy.actor.TypedIORelation">
        </relation>
        <link port="output" relation="r1"/>
        <link port="a.input" relation="r1"/>
        <link port="a.multi" relation="r1"/>
    </entity>
</entity>
}

##

test UndoLink-1.1b {Test undoing a link: outside link, given relation, no index} {
    # Now create the MoMLUndoChangeRequest which will undo the change
    set undochange [java::new ptolemy.kernel.undo.UndoChangeRequest $toplevel $toplevel]

    # NOTE: Request is filled immediately because the model is not running.
    $manager requestChange $undochange

    # Should be back to the base model...
    $toplevel exportMoML
} {<?xml version="1.0" standalone="no"?>
<!DOCTYPE entity PUBLIC "-//UC Berkeley//DTD MoML 1//EN"
    "http://ptolemy.eecs.berkeley.edu/xml/dtd/MoML_1.dtd">
<entity name="top" class="ptolemy.actor.TypedCompositeActor">
    <entity name="level" class="ptolemy.actor.TypedCompositeActor">
        <port name="output" class="ptolemy.actor.TypedIOPort">
            <property name="output"/>
            <property name="multiport"/>
        </port>
        <entity name="a" class="ptolemy.actor.TypedCompositeActor">
            <port name="input" class="ptolemy.actor.TypedIOPort">
                <property name="input"/>
            </port>
            <port name="multi" class="ptolemy.actor.TypedIOPort">
                <property name="multiport"/>
            </port>
        </entity>
        <relation name="r1" class="ptolemy.actor.TypedIORelation">
        </relation>
        <link port="output" relation="r1"/>
        <link port="a.input" relation="r1"/>
    </entity>
</entity>
}

######################################################################
####
#

test UndoLink-1.2a {Test undoing a link: outside link, given relation, with index} {
   set change [java::new ptolemy.moml.MoMLChangeRequest $toplevel $toplevel {
        <entity name=".top.level">
            <link port="a.multi" relation="r1" insertAt="3" />
        </entity>
    }]
    # Mark the change as being undoable
    $change setUndoable true

    # NOTE: Request is filled immediately because the model is not running.
    $manager requestChange $change

    # Export the modified MoML
    $toplevel exportMoML
} {<?xml version="1.0" standalone="no"?>
<!DOCTYPE entity PUBLIC "-//UC Berkeley//DTD MoML 1//EN"
    "http://ptolemy.eecs.berkeley.edu/xml/dtd/MoML_1.dtd">
<entity name="top" class="ptolemy.actor.TypedCompositeActor">
    <entity name="level" class="ptolemy.actor.TypedCompositeActor">
        <port name="output" class="ptolemy.actor.TypedIOPort">
            <property name="output"/>
            <property name="multiport"/>
        </port>
        <entity name="a" class="ptolemy.actor.TypedCompositeActor">
            <port name="input" class="ptolemy.actor.TypedIOPort">
                <property name="input"/>
            </port>
            <port name="multi" class="ptolemy.actor.TypedIOPort">
                <property name="multiport"/>
            </port>
        </entity>
        <relation name="r1" class="ptolemy.actor.TypedIORelation">
        </relation>
        <link port="output" relation="r1"/>
        <link port="a.input" relation="r1"/>
        <link port="a.multi" insertAt="3" relation="r1"/>
    </entity>
</entity>
}

##

test UndoLink-1.2b {Test undoing a link: outside link, given relation, with index} {
    # Now create the MoMLUndoChangeRequest which will undo the change
    set undochange [java::new ptolemy.kernel.undo.UndoChangeRequest $toplevel $toplevel]

    # NOTE: Request is filled immediately because the model is not running.
    $manager requestChange $undochange

    # Should be back to the base model...
    $toplevel exportMoML
} {<?xml version="1.0" standalone="no"?>
<!DOCTYPE entity PUBLIC "-//UC Berkeley//DTD MoML 1//EN"
    "http://ptolemy.eecs.berkeley.edu/xml/dtd/MoML_1.dtd">
<entity name="top" class="ptolemy.actor.TypedCompositeActor">
    <entity name="level" class="ptolemy.actor.TypedCompositeActor">
        <port name="output" class="ptolemy.actor.TypedIOPort">
            <property name="output"/>
            <property name="multiport"/>
        </port>
        <entity name="a" class="ptolemy.actor.TypedCompositeActor">
            <port name="input" class="ptolemy.actor.TypedIOPort">
                <property name="input"/>
            </port>
            <port name="multi" class="ptolemy.actor.TypedIOPort">
                <property name="multiport"/>
            </port>
        </entity>
        <relation name="r1" class="ptolemy.actor.TypedIORelation">
        </relation>
        <link port="output" relation="r1"/>
        <link port="a.input" relation="r1"/>
    </entity>
</entity>
}

######################################################################
####
#

test UndoLink-1.3a {Test undoing a link: outside link, no relation, with index} {
   set change [java::new ptolemy.moml.MoMLChangeRequest $toplevel $toplevel {
        <entity name=".top.level">
            <link port="a.multi" insertAt="0" relation="r1"/>
            <link port="a.multi" insertAt="0" />
        </entity>
    }]
    # Mark the change as being undoable
    $change setUndoable true

    # NOTE: Request is filled immediately because the model is not running.
    $manager requestChange $change

    # Export the modified MoML
    $toplevel exportMoML
} {<?xml version="1.0" standalone="no"?>
<!DOCTYPE entity PUBLIC "-//UC Berkeley//DTD MoML 1//EN"
    "http://ptolemy.eecs.berkeley.edu/xml/dtd/MoML_1.dtd">
<entity name="top" class="ptolemy.actor.TypedCompositeActor">
    <entity name="level" class="ptolemy.actor.TypedCompositeActor">
        <port name="output" class="ptolemy.actor.TypedIOPort">
            <property name="output"/>
            <property name="multiport"/>
        </port>
        <entity name="a" class="ptolemy.actor.TypedCompositeActor">
            <port name="input" class="ptolemy.actor.TypedIOPort">
                <property name="input"/>
            </port>
            <port name="multi" class="ptolemy.actor.TypedIOPort">
                <property name="multiport"/>
            </port>
        </entity>
        <relation name="r1" class="ptolemy.actor.TypedIORelation">
        </relation>
        <link port="output" relation="r1"/>
        <link port="a.input" relation="r1"/>
        <link port="a.multi" insertAt="1" relation="r1"/>
    </entity>
</entity>
}

##

test UndoLink-1.3b {Test undoing a link: outside link, no relation, with index} {
    # Now create the MoMLUndoChangeRequest which will undo the change
    set undochange [java::new ptolemy.kernel.undo.UndoChangeRequest $toplevel $toplevel]

    # NOTE: Request is filled immediately because the model is not running.
    $manager requestChange $undochange

    # Should be back to the base model...
    $toplevel exportMoML
} {<?xml version="1.0" standalone="no"?>
<!DOCTYPE entity PUBLIC "-//UC Berkeley//DTD MoML 1//EN"
    "http://ptolemy.eecs.berkeley.edu/xml/dtd/MoML_1.dtd">
<entity name="top" class="ptolemy.actor.TypedCompositeActor">
    <entity name="level" class="ptolemy.actor.TypedCompositeActor">
        <port name="output" class="ptolemy.actor.TypedIOPort">
            <property name="output"/>
            <property name="multiport"/>
        </port>
        <entity name="a" class="ptolemy.actor.TypedCompositeActor">
            <port name="input" class="ptolemy.actor.TypedIOPort">
                <property name="input"/>
            </port>
            <port name="multi" class="ptolemy.actor.TypedIOPort">
                <property name="multiport"/>
            </port>
        </entity>
        <relation name="r1" class="ptolemy.actor.TypedIORelation">
        </relation>
        <link port="output" relation="r1"/>
        <link port="a.input" relation="r1"/>
    </entity>
</entity>
}

######################################################################
####
#

test UndoLink-1.4a {Test undoing a link: inside link, no relation, with
                        index having no effect} {
   set change [java::new ptolemy.moml.MoMLChangeRequest $toplevel $toplevel {
        <entity name=".top.level">
            <link port="output" insertInsideAt="3" />
        </entity>
    }]
    # Mark the change as being undoable
    $change setUndoable true

    # NOTE: Request is filled immediately because the model is not running.
    $manager requestChange $change

    # Export the modified MoML
    $toplevel exportMoML
} {<?xml version="1.0" standalone="no"?>
<!DOCTYPE entity PUBLIC "-//UC Berkeley//DTD MoML 1//EN"
    "http://ptolemy.eecs.berkeley.edu/xml/dtd/MoML_1.dtd">
<entity name="top" class="ptolemy.actor.TypedCompositeActor">
    <entity name="level" class="ptolemy.actor.TypedCompositeActor">
        <port name="output" class="ptolemy.actor.TypedIOPort">
            <property name="output"/>
            <property name="multiport"/>
        </port>
        <entity name="a" class="ptolemy.actor.TypedCompositeActor">
            <port name="input" class="ptolemy.actor.TypedIOPort">
                <property name="input"/>
            </port>
            <port name="multi" class="ptolemy.actor.TypedIOPort">
                <property name="multiport"/>
            </port>
        </entity>
        <relation name="r1" class="ptolemy.actor.TypedIORelation">
        </relation>
        <link port="output" relation="r1"/>
        <link port="a.input" relation="r1"/>
    </entity>
</entity>
}

##

test UndoLink-1.4b {Test undoing a link: inside link, no relation, with
                        index having no effect} {
    # Now create the MoMLUndoChangeRequest which will undo the change
    set undochange [java::new ptolemy.kernel.undo.UndoChangeRequest $toplevel $toplevel]

    # NOTE: Request is filled immediately because the model is not running.
    $manager requestChange $undochange

    # Should be back to the base model...
    $toplevel exportMoML
} {<?xml version="1.0" standalone="no"?>
<!DOCTYPE entity PUBLIC "-//UC Berkeley//DTD MoML 1//EN"
    "http://ptolemy.eecs.berkeley.edu/xml/dtd/MoML_1.dtd">
<entity name="top" class="ptolemy.actor.TypedCompositeActor">
    <entity name="level" class="ptolemy.actor.TypedCompositeActor">
        <port name="output" class="ptolemy.actor.TypedIOPort">
            <property name="output"/>
            <property name="multiport"/>
        </port>
        <entity name="a" class="ptolemy.actor.TypedCompositeActor">
            <port name="input" class="ptolemy.actor.TypedIOPort">
                <property name="input"/>
            </port>
            <port name="multi" class="ptolemy.actor.TypedIOPort">
                <property name="multiport"/>
            </port>
        </entity>
        <relation name="r1" class="ptolemy.actor.TypedIORelation">
        </relation>
        <link port="output" relation="r1"/>
        <link port="a.input" relation="r1"/>
    </entity>
</entity>
}

######################################################################
####
#

test UndoLink-1.5a {Test undoing a link: inside link, no relation, with
                        index having an effect} {
   set change [java::new ptolemy.moml.MoMLChangeRequest $toplevel $toplevel {
        <entity name=".top.level">
            <link port="output" insertInsideAt="0" />
        </entity>
    }]
    # Mark the change as being undoable
    $change setUndoable true

    # NOTE: Request is filled immediately because the model is not running.
    $manager requestChange $change

    # Export the modified MoML
    $toplevel exportMoML
} {<?xml version="1.0" standalone="no"?>
<!DOCTYPE entity PUBLIC "-//UC Berkeley//DTD MoML 1//EN"
    "http://ptolemy.eecs.berkeley.edu/xml/dtd/MoML_1.dtd">
<entity name="top" class="ptolemy.actor.TypedCompositeActor">
    <entity name="level" class="ptolemy.actor.TypedCompositeActor">
        <port name="output" class="ptolemy.actor.TypedIOPort">
            <property name="output"/>
            <property name="multiport"/>
        </port>
        <entity name="a" class="ptolemy.actor.TypedCompositeActor">
            <port name="input" class="ptolemy.actor.TypedIOPort">
                <property name="input"/>
            </port>
            <port name="multi" class="ptolemy.actor.TypedIOPort">
                <property name="multiport"/>
            </port>
        </entity>
        <relation name="r1" class="ptolemy.actor.TypedIORelation">
        </relation>
        <link port="output" insertAt="1" relation="r1"/>
        <link port="a.input" relation="r1"/>
    </entity>
</entity>
}

##

test UndoLink-1.5b {TTest undoing a link: inside link, no relation, with
                        index having an effect} {
    # Now create the MoMLUndoChangeRequest which will undo the change
    set undochange [java::new ptolemy.kernel.undo.UndoChangeRequest $toplevel $toplevel]

    # NOTE: Request is filled immediately because the model is not running.
    $manager requestChange $undochange

    # Should be back to the base model...
    $toplevel exportMoML
} {<?xml version="1.0" standalone="no"?>
<!DOCTYPE entity PUBLIC "-//UC Berkeley//DTD MoML 1//EN"
    "http://ptolemy.eecs.berkeley.edu/xml/dtd/MoML_1.dtd">
<entity name="top" class="ptolemy.actor.TypedCompositeActor">
    <entity name="level" class="ptolemy.actor.TypedCompositeActor">
        <port name="output" class="ptolemy.actor.TypedIOPort">
            <property name="output"/>
            <property name="multiport"/>
        </port>
        <entity name="a" class="ptolemy.actor.TypedCompositeActor">
            <port name="input" class="ptolemy.actor.TypedIOPort">
                <property name="input"/>
            </port>
            <port name="multi" class="ptolemy.actor.TypedIOPort">
                <property name="multiport"/>
            </port>
        </entity>
        <relation name="r1" class="ptolemy.actor.TypedIORelation">
        </relation>
        <link port="output" relation="r1"/>
        <link port="a.input" relation="r1"/>
    </entity>
</entity>
}

######################################################################
####
#

test UndoLink-1.6a {Test undoing a link: inside link, with relation, with
                        index} {
   set change [java::new ptolemy.moml.MoMLChangeRequest $toplevel $toplevel {
        <entity name=".top.level">
            <link port="output" relation="r1" insertInsideAt="3" />
        </entity>
    }]
    # Mark the change as being undoable
    $change setUndoable true

    # NOTE: Request is filled immediately because the model is not running.
    $manager requestChange $change

    # Export the modified MoML
    $toplevel exportMoML
} {<?xml version="1.0" standalone="no"?>
<!DOCTYPE entity PUBLIC "-//UC Berkeley//DTD MoML 1//EN"
    "http://ptolemy.eecs.berkeley.edu/xml/dtd/MoML_1.dtd">
<entity name="top" class="ptolemy.actor.TypedCompositeActor">
    <entity name="level" class="ptolemy.actor.TypedCompositeActor">
        <port name="output" class="ptolemy.actor.TypedIOPort">
            <property name="output"/>
            <property name="multiport"/>
        </port>
        <entity name="a" class="ptolemy.actor.TypedCompositeActor">
            <port name="input" class="ptolemy.actor.TypedIOPort">
                <property name="input"/>
            </port>
            <port name="multi" class="ptolemy.actor.TypedIOPort">
                <property name="multiport"/>
            </port>
        </entity>
        <relation name="r1" class="ptolemy.actor.TypedIORelation">
        </relation>
        <link port="output" relation="r1"/>
        <link port="output" insertAt="3" relation="r1"/>
        <link port="a.input" relation="r1"/>
    </entity>
</entity>
}

##

test UndoLink-1.6b {Test undoing a link: inside link, with relation, with
                        index} {
    # Now create the MoMLUndoChangeRequest which will undo the change
    set undochange [java::new ptolemy.kernel.undo.UndoChangeRequest $toplevel $toplevel]

    # NOTE: Request is filled immediately because the model is not running.
    $manager requestChange $undochange

    # Should be back to the base model...
    $toplevel exportMoML
} {<?xml version="1.0" standalone="no"?>
<!DOCTYPE entity PUBLIC "-//UC Berkeley//DTD MoML 1//EN"
    "http://ptolemy.eecs.berkeley.edu/xml/dtd/MoML_1.dtd">
<entity name="top" class="ptolemy.actor.TypedCompositeActor">
    <entity name="level" class="ptolemy.actor.TypedCompositeActor">
        <port name="output" class="ptolemy.actor.TypedIOPort">
            <property name="output"/>
            <property name="multiport"/>
        </port>
        <entity name="a" class="ptolemy.actor.TypedCompositeActor">
            <port name="input" class="ptolemy.actor.TypedIOPort">
                <property name="input"/>
            </port>
            <port name="multi" class="ptolemy.actor.TypedIOPort">
                <property name="multiport"/>
            </port>
        </entity>
        <relation name="r1" class="ptolemy.actor.TypedIORelation">
        </relation>
        <link port="output" relation="r1"/>
        <link port="a.input" relation="r1"/>
    </entity>
</entity>
}