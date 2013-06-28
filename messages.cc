/*
 *  messages.cc
 *
 *  Copyright (C) 1998	Jiann-Ching Liu
 */

#include "messages.h"

const char *chinese_message[] = {
    "需要 block 形式的標記區",
    "不能儲存內部檔案",
    "更換",
    "確定要修改 [y/n/q/a]:",
    "與現存的標記區衝突",
    "您真得想退出嗎? 請打 y 或 n",
    "開檔錯誤",
    "本檔案是唯讀狀態",
    "檔案儲存",
    "檔案尚未命名",
    "檔名已在使用中",
    "新檔",
    "沒有變更",
    "沒有標記區",
    "沒有已儲存的標記區",
    "沒有找到!",
    "沒有找到! ( Ctrl-G 停止 )",
    "記憶體不足!",
    "權限不足!",
    "跳出",
    "唯讀檔，強迫寫入? 請打 y 或 n",
    "讀取錯誤: 權限不足!",
    "重頭搜尋! ( Ctrl-G 停止 )",
    "來源和目的衝突",
    "已儲存太多標記區",
    "敲入一個字:",
    "不認識的命令",
    "請按 ENTER 選擇，或是 SPACE 放棄",
    "命令列的歷史記錄: 請按 ENTER 選擇，或是 SPACE 放棄"
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

