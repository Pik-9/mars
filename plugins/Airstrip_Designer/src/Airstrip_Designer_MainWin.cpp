/*
 *  Copyright 2013, DFKI GmbH Robotics Innovation Center
 *
 *  This file is part of the MARS simulation framework.
 *
 *  MARS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  MARS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with MARS.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * \file Airstrip_Designer.cpp
 * \author Daniel Steinhauer (d.steinhauer@mailbox.org)
 * \brief A plugin which can create design an airstrip with obstacles.
 *
 * Version 0.1
 */

#include "Airstrip_Designer_MainWin.h"

Airstrip_Designer_MainWin::Airstrip_Designer_MainWin ()
  : QWidget ()
{
  resize (500, 300);
  setWindowTitle ("Airstrip_Designer");
  
  msg = new QLabel (tr ("Hello DFKI!"));
  btn = new QPushButton (tr ("Close"));
  lyt = new QGridLayout (this);
  
  lyt->addWidget (msg, 0, 0, 1, 3);
  lyt->addWidget (btn, 1, 1, 1, 1);
  
  connect (btn, SIGNAL (clicked ()), this, SLOT (hide ()));
}

Airstrip_Designer_MainWin::~Airstrip_Designer_MainWin ()
{
  delete msg;
  delete btn;
  delete lyt;
}