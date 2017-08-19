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

stock GetPlayerLevel(level)
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