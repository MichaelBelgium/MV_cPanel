forward OnAccountCheck(playerid);

public OnAccountCheck(playerid)
{
	if(cache_num_rows() == 1)
		ShowPlayerDialogEx(playerid,DIALOG_LOGIN);
	else
		ShowPlayerDialogEx(playerid,DIALOG_REGISTER);
}