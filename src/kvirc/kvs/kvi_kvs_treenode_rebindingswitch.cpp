//=============================================================================
//
//   File : kvi_kvs_treenode_rebindingswitch.cpp
//   Creation date : Mon 15 Aug 2005 13:32:31 by Szymon Stefanek
//
//   This file is part of the KVIrc IRC Client distribution
//   Copyright (C) 2005-2008 Szymon Stefanek <pragma at kvirc dot net>
//
//   This program is FREE software. You can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your opinion) any later version.
//
//   This program is distributed in the HOPE that it will be USEFUL,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program. If not, write to the Free Software Foundation,
//   Inc. ,59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
//=============================================================================

#include "kvi_kvs_treenode_rebindingswitch.h"
#include "kvi_kvs_treenode_data.h"
#include "kvi_window.h"
#include "kvi_app.h"
#include "kvi_locale.h"
#include "kvi_kvs_runtimecontext.h"

KviKvsTreeNodeRebindingSwitch::KviKvsTreeNodeRebindingSwitch(const QChar * pLocation,KviKvsTreeNodeData * pTargetWindow,KviKvsTreeNodeCommand * pChildCommand)
: KviKvsTreeNodeCommand(pLocation,pChildCommand->commandName())
{
	m_pTargetWindow = pTargetWindow;
	m_pTargetWindow->setParent(this);
	m_pChildCommand = pChildCommand;
	m_pChildCommand->setParent(this);
}

KviKvsTreeNodeRebindingSwitch::~KviKvsTreeNodeRebindingSwitch()
{
	delete m_pTargetWindow;
	delete m_pChildCommand;
}


void KviKvsTreeNodeRebindingSwitch::contextDescription(QString &szBuffer)
{
	szBuffer = "Window Rebinding Switch";
}

void KviKvsTreeNodeRebindingSwitch::dump(const char * prefix)
{
	qDebug("%sRebindingSwitch",prefix);
	QString tmp = prefix;
	tmp += "  ";
	m_pTargetWindow->dump(tmp.toUtf8().data());
	m_pChildCommand->dump(tmp.toUtf8().data());
}

const QString & KviKvsTreeNodeRebindingSwitch::commandName()
{
	return m_pChildCommand->commandName();
}

bool KviKvsTreeNodeRebindingSwitch::execute(KviKvsRunTimeContext * c)
{
	KviKvsVariant vWindow;
	if(!m_pTargetWindow->evaluateReadOnly(c,&vWindow))return false;

	KviWindow * pNewWindow;

	QString szWinId;
	vWindow.asString(szWinId);
	if(szWinId.isEmpty())
	{
		c->warning(this,__tr2qs_ctx("Empty window identifier specified in the standard rebinding switch: no rebinding performed","kvs"));
		pNewWindow = 0;
	} else {
		pNewWindow = g_pApp->findWindow(szWinId.toUtf8().data());
	}

	pNewWindow = g_pApp->findWindow(szWinId.toUtf8().data());
	KviWindow * pOldWindow = c->window();
	if(pNewWindow)
		c->setWindow(pNewWindow);
	else
		c->warning(this,__tr2qs_ctx("Invalid window specified in the standard rebinding switch: no rebinding performed","kvs"));
	bool bRet = m_pChildCommand->execute(c);
	c->setWindow(pOldWindow);
	return bRet;
}
