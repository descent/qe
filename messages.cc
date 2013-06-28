/*
 *  messages.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#include "messages.h"

const char *chinese_message[] = {
    "�ݭn block �Φ����аO��",
    "�����x�s�����ɮ�",
    "��",
    "�T�w�n�ק� [y/n/q/a]:",
    "�P�{�s���аO�ϽĬ�",
    "�z�u�o�Q�h�X��? �Х� y �� n",
    "�}�ɿ��~",
    "���ɮ׬O��Ū���A",
    "�ɮ��x�s",
    "�ɮש|���R�W",
    "�ɦW�w�b�ϥΤ�",
    "�s��",
    "�S���ܧ�",
    "�S���аO��",
    "�S���w�x�s���аO��",
    "�S�����!",
    "�S�����! ( Ctrl-G ���� )",
    "�O���餣��!",
    "�v������!",
    "���X",
    "��Ū�ɡA�j���g�J? �Х� y �� n",
    "Ū�����~: �v������!",
    "���Y�j�M! ( Ctrl-G ���� )",
    "�ӷ��M�ت��Ĭ�",
    "�w�x�s�Ӧh�аO��",
    "�V�J�@�Ӧr:",
    "���{�Ѫ��R�O",
    "�Ы� ENTER ��ܡA�άO SPACE ���",
    "�R�O�C�����v�O��: �Ы� ENTER ��ܡA�άO SPACE ���"
};

const char *english_message[] = {
    "Block mark required",			     // MSG_BLOCK_MARK_REQ
    "Cannot save internal file",		     // MSG_CANT_SAVE_INTERNAL
    "Change it",				     // MSG_CHANGE_IT
    "Confirm change [y/n/q/a]:",		     // MSG_CONFIRM_CHANGE
    "Conflict with existing mark",		     // MSG_CONFLICT_EXIST_MARK
    "Do you really want to quit? Type y or n",	     // MSG_REALLY_QUIT
    "Error opening file",			     // MSG_ERROR_OPEN_FILE
    "File is read only",			     // MSG_READ_ONLY_FILE
    "File saved",				     // MSG_FILE_SAVED
    "Missing file name",			     // MSG_MESSING_FILE_NAME
    "Name already in use",			     // MSG_NAME_IN_USE
    "New file", 				     // MSG_NEW_FILE
    "No change",				     // MSG_NO_CHANGE
    "No marked area",				     // MSG_NO_MARKED_AREA
    "No marked blocks saved",			     // MSG_NO_MARK_SAVED
    "No match found!",				     // MSG_NO_MATCH_FOUND
    "No match found! ( Ctrl-G to stop )",	     // MSG_NO_MATCH_CTRL_G
    "Out of memory!",				     // MSG_OUT_OF_MEMORY
    "Permission Deny!", 			     // MSG_PERMISSION_DENY
    "Quit",					     // MSG_QUIT
    "Read-only file, write force? Type y or n",      // MSG_WRITE_FORCE
    "Reading error: Permission deny!",		     // MSG_READ_ERROR
    "Search wrapped! ( Ctrl-G to stop )",	     // MSG_SEARCH_WRAPPED
    "Source and target conflict",		     // MSG_S_T_CONFLICT
    "Too many marked blocks saved",		     // MSG_TOO_MANY_MARK_SAVE
    "Type a character:",			     // MSG_TYPE_A_CHARACTER
    "Unknow command",				     // MSG_UNKNOW_COMMAND
    "Press ENTER to select, and SPACE to quit",      // MSG_MENU_USAGE
    "Command History: Press ENTER to select, and SPACE to quit"       // MSG_CMD_HISTORY
};

