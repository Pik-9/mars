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
 * \file Airstrip_Designer.h
 * \author Daniel Steinhauer (d.steinhauer@mailbox.org)
 * \brief A plugin which can create design an airstrip with obstacles.
 *
 * Version 0.1
 */

#ifndef MARS_PLUGINS_AIRSTRIP_DESIGNER_HMW
#define MARS_PLUGINS_AIRSTRIP_DESIGNER_HMW

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

class Airstrip_Designer_MainWin : public QWidget
{
private:
  QLabel *msg;
  QPushButton *btn;
  QGridLayout *lyt;
  
public:
  Airstrip_Designer_MainWin ();
  virtual ~Airstrip_Designer_MainWin ();
};

#endif