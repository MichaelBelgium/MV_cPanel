ShowPlayerDialogEx(playerid,dialogid)
{
	new string[128];
	switch(dialogid)
	{
		case DIALOG_LOGIN:
		{
			format(string, sizeof(string), COL_WHITE"Your user is "COL_RED"registered"COL_WHITE"! Please "COL_BLUE"login"COL_WHITE" with your password below!", PlayerInfo[playerid][Name]);
			ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", string, "Login", "");
		}

		case DIALOG_REGISTER:
		{
			format(string, sizeof(string), COL_WHITE"Your user (%s) is "COL_RED"not"COL_WHITE" registered! Please "COL_BLUE"register"COL_WHITE" with a password below!", PlayerInfo[playerid][Name]);
			ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register", string, "Register", "");
		}
	}
}