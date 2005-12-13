/****************************************************************************
** Form implementation generated from reading ui file 'brow.ui'
**
** Created: Mon Dec 5 15:28:41 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "HelpBrowser.h"

#include <qvariant.h>
#include <qtoolbutton.h>
#include <qtextbrowser.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a Form1 as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
HelpBrowser::HelpBrowser( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "Form1" );

    layout4 = new QVBoxLayout( this, 11, 6, "layout4"); 

    layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 

    _backBut = new QToolButton( this, "_backBut" );
    layout3->addWidget( _backBut );

    _forwardBut = new QToolButton( this, "_forwardBut" );
    layout3->addWidget( _forwardBut );
    spacer1 = new QSpacerItem( 130, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer1 );

    _editBut = new QToolButton( this, "_editBut" );
    layout3->addWidget( _editBut );
    layout4->addLayout( layout3 );

    _textBrowser = new QTextBrowser( this, "_textBrowser" );
    layout4->addWidget( _textBrowser );
    _backBut->setText( tr( "Back" ) );
    _forwardBut->setText( tr( "Forward" ) );
    _editBut->setText( tr( "Edit" ) );resize( QSize(600, 480).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
    connect(_editBut,SIGNAL(clicked()),SIGNAL(edit()));

}

/*
 *  Destroys the object and frees any allocated resources
 */
HelpBrowser::~HelpBrowser()
{
    // no need to delete child widgets, Qt does it all for us
}

void HelpBrowser::setText(QString txt)
{
  _textBrowser->setText(txt);
}