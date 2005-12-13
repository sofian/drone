#include "HelpEditor.h"
#include <qvariant.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qcombobox.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qpair.h>
/*
 *  Constructs a HelpEditor as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
HelpEditor::HelpEditor( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QWidget( parent, name, fl )
{
  if ( !name )
    setName( "HelpEditor" );

  layout6 = new QVBoxLayout( this, 11, 6, "layout6");
  QHBoxLayout * layout116 = new QHBoxLayout( layout6);
  textLabel3 = new QLabel( this, "textLabel3" );
  textLabel3->setText( tr( "Intro:" ) );

  layout116->addWidget( textLabel3 );
  layout116->addStretch(99);
  pushButton1 = new QToolButton( this, "pushButton1" );
  pushButton1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, pushButton1->sizePolicy().hasHeightForWidth() ) );
  pushButton1->setText( tr( "Save" ) );

  layout116->addWidget( pushButton1 );

  _introEditor = new QTextEdit( this, "_introEditor" );
  layout6->addWidget( _introEditor );

  layout2 = new QHBoxLayout( 0, 0, 6, "layout2");

  textLabel3_2 = new QLabel( this, "textLabel3_2" );
  textLabel3_2->setText( tr( "Plugs:" ) );

  layout2->addWidget( textLabel3_2 );

  _plugsCombo = new QComboBox( FALSE, this, "_plugsCombo" );
  _plugsCombo->setEditable( FALSE );
  layout2->addWidget( _plugsCombo );
  layout6->addLayout( layout2 );

  _plugEditor = new QTextEdit( this, "_plugEditor" );
  layout6->addWidget( _plugEditor );

  textLabel4 = new QLabel( this, "textLabel4" );
  textLabel4->setText( tr( "Description:" ) );

  layout6->addWidget( textLabel4 );

  _descriptionEditor = new QTextEdit( this, "_descriptionEditor" );
  layout6->addWidget( _descriptionEditor );

  QToolTip::add( _introEditor, tr( "Enter a short and intuitive description of the gears" ) );
  QToolTip::add( _descriptionEditor, tr( "Enter a longer and more extensive description of the gear" ) );
  connect(pushButton1,SIGNAL(clicked()),SLOT(save()));
  resize( QSize(444, 518).expandedTo(minimumSizeHint()) );
  clearWState( WState_Polished );
  connect(_plugsCombo,SIGNAL(highlighted(int)),this,SLOT(plugSelected(int)));
  _currentPlugInfo=NULL;
}

/*
 *  Destroys the object and frees any allocated resources
 */
HelpEditor::~HelpEditor()
{
  // no need to delete child widgets, Qt does it all for us
}

void HelpEditor::plugSelected(int i)
{
  if(_currentPlugInfo)
    _currentPlugInfo->help=_plugEditor->text();
  _plugEditor->setText(_pimap[i].second->help);
  _currentPlugInfo=_pimap[i].second;
}

void HelpEditor::loadGearInfo(GearInfo_* gi)
{
  _introEditor->setText(gi->_intro);
  _descriptionEditor->setText(gi->_description);
  QMap<QString,PlugInfo>::Iterator it;
  _plugsCombo->clear();
  _pimap.clear();
  int index=0;
  for ( it = gi->_plugs.begin(); it != gi->_plugs.end(); ++it )
  {
    _plugsCombo->insertItem(it.key()+" ["+(it.data().inOut==IN?QString("IN"):QString("OUT"))+","+it.data().type+"]",index);
    _pimap[index]=qMakePair(it.key(),&it.data());
  }

  _currentGearInfo=gi;

  std::cerr<<"loaded gearinfo...:"<<_currentGearInfo<<std::endl;

  _currentPlugInfo=_pimap[_plugsCombo->currentItem()].second;
}

void HelpEditor::save()
{
  // save help
  std::cerr<<"updating gearinfo...intro:"<<_introEditor->text()<<std::endl;

  std::cerr<<"using gearinfo...:"<<_currentGearInfo<<std::endl;

  _currentGearInfo->_intro = _introEditor->text();
  _currentGearInfo->_description = _descriptionEditor->text();

  _currentGearInfo->_plugs.clear();
  std::cerr<<"updating gearinfo...plugs"<<std::endl;

  QMap<int,QPair<QString,PlugInfo*> >::Iterator it;
  for ( it = _pimap.begin(); it != _pimap.end(); ++it )
    1;//std::cerr<<it.data().second<<std::endl;
//_currentGearInfo->setPlugInfo("",it.data().second);

  std::cerr<<"DONE"<<std::endl;
_currentGearInfo->saveDefinition();
  emit saved();

}
