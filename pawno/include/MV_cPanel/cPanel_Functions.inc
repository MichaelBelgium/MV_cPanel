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

        case DIALOG_CPANEL: ShowPlayerDialog(playerid,DIALOG_CPANEL,DIALOG_STYLE_LIST,"cPanel by {FF0A00}Michael@Belgium","Open player panel\nOpen server panel\nRestart server","Ok","Cancel");
        case DIALOG_CPANEL_PLAYERPANEL: ShowPlayerDialog(playerid,DIALOG_CPANEL_PLAYERPANEL,DIALOG_STYLE_INPUT,"Player ID","Please fill in a PlayerID.","OK","Cancel");
        case DIALOG_CPANEL_SERVERPANEL: ShowPlayerDialog(playerid,DIALOG_CPANEL_SERVERPANEL,DIALOG_STYLE_LIST,"cPanel by {FF0A00}Michael@Belgium","Change gamemode \r\nLoad filterscript \r\nUnload filterscript \r\nSet hostname \r\nSet server password \r\nSet gamemodetext","Select","Back");
	}

    return 1;
}

SendClientMessageToAdmins(color, message[])
{
    for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
    {
        if(!IsPlayerConnected(i) || PlayerInfo[i][Adminlevel] == 0) continue;
        SendClientMessage(i, color, message);
    }
}

stock GetPlayerLevel(playerid) return PlayerInfo[playerid][Adminlevel];

stock GetPlayerLevelEx(level)
{
    new playerlevel[32];
    switch(level)
    {
        case LEVEL_TRIAL_MOD: playerlevel = "Trial moderator";
        case LEVEL_MOD: playerlevel = "Moderator";
        case LEVEL_TRIAL_ADMIN: playerlevel = "Trial administrator";
        case LEVEL_ADMIN: playerlevel = "Administrator";
        case LEVEL_OWNER: playerlevel = "Owner";
    }
    return playerlevel;
}

stock IsPlayerVIP(playerid) return VipInfo[playerid][Duration] != -1;
stock IsPlayerAdminEx(playerid, level) return PlayerInfo[playerid][Adminlevel] >= level;

stock GivePlayerScore(playerid, score)
{
    PlayerInfo[playerid][Score] += score;
    SetPlayerScore(playerid, PlayerInfo[playerid][Score]);
}

stock GivePlayerCash(playerid, money)
{
    PlayerInfo[playerid][Money] += money;

    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, PlayerInfo[playerid][Money]);
}

stock SetPlayerCash(playerid, money)
{
    PlayerInfo[playerid][Money] = money;

    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, PlayerInfo[playerid][Money]);
}

stock GetPlayerCash(playerid) return PlayerInfo[playerid][Money];

stock UnlockVehicle(vehicleid) 
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, VEHICLE_PARAMS_OFF, bonnet, boot, objective);
}

stock LockVehicle(vehicleid) 
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, VEHICLE_PARAMS_ON, bonnet, boot, objective);
}

stock MsToString(millisecond, format[])
{
    new
        tmp[4],
        result[64],
        hour,
        minute,
        second,
        format_char,
        result_lenght,
        bool:padding,
        len = strlen(format);
 
    hour            = (millisecond / (1000 * 60 * 60));
    minute          = (millisecond % (1000 * 60 * 60)) / (1000 * 60);
    second          = ((millisecond % (1000 * 60 * 60)) % (1000 * 60)) / 1000;
    millisecond     = millisecond - (hour * 60 * 60 * 1000) - (minute * 60 * 1000) - (second * 1000);
 
    while(format_char < len)
    {
        if(format[format_char] == '%')
        {
            format_char++;
 
            if(format[format_char] == '1')
            {
                padding = true;
                format_char++;
            }
            else
            {
                padding = false;
            }
 
            switch(format[format_char])
            {
                case 'h':
                {
                    valstr(tmp, hour);
 
                    if(padding)
                    {
                        if(hour < 10)
                            strcat(result, "0");
                    }
 
                    strcat(result, tmp);
                    result_lenght = strlen(result);
                }
 
                case 'm':
                {
                    valstr(tmp, minute);
 
                    if(padding)
                    {
                        if(minute < 10)
                            strcat(result, "0");
                    }
 
                    strcat(result, tmp);
                    result_lenght = strlen(result);
                }
 
                case 's':
                {
                    valstr(tmp, second);
 
                    if(padding)
                    {
                        if(second < 10)
                            strcat(result, "0");
                    }
 
                    strcat(result, tmp);
                    result_lenght = strlen(result);
                }
 
                case 'd':
                {
                    valstr(tmp, millisecond);
 
                    if(padding)
                    {
                        if(millisecond < 10)
                            strcat(result, "00");
 
                        else if(millisecond < 100)
                            strcat(result, "0");
                    }
 
                    strcat(result, tmp);
                    result_lenght = strlen(result);
                }
            }
        }
        else
        {
            result[result_lenght] = format[format_char];
            result_lenght++;
        }
 
        format_char++;
    }
 
    return result;
}