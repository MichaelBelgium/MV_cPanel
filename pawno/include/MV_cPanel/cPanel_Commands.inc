CMD:cmds(playerid,params[])
{
	new cmds[256*3];

	strcat(cmds, "General:\nstats admins report\n\n");

	if(IsPlayerVIP(playerid)) strcat(cmds, "Vips:\nvipveh viprep viprepair viprep vipnos vipflip vipunlimnos vipheal vipgodcar viplock\n\n");
	if(IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) strcat(cmds, "Trial mod:\nwarn asay crew eject mute unmute weaponids slap spectate disarm adminduty\n\n");
	if(IsPlayerAdminEx(playerid, LEVEL_MOD)) strcat(cmds, "Moderator:\nannounce kick jail unjail facepunch freeze unfreeze burn crash ajetpack\n\n");
	if(IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) strcat(cmds, "Trial administrator:\ncage uncage get goto ban setinterior spawn sethealth setarmour healall setskin giveweapon explode settime setweather reconnect setallskin setalltime setallweather\n\n");
	if(IsPlayerAdminEx(playerid, LEVEL_ADMIN)) strcat(cmds, "Administrator:\nsetscore setmoney armourall setallscore spawnall giveallweapon killall getall ejectall godall ungodall freezeall unfreezeall\n\n");
	if(IsPlayerAdminEx(playerid, LEVEL_OWNER)) strcat(cmds, "Owner:\nsetlevel fakechat makevip removevip\n");

	ShowPlayerDialog(playerid, DIALOG_NORESPONSE, DIALOG_STYLE_MSGBOX, "Available commands:", cmds, "OK", "");
	return 1;
}

CMD:commands(playerid, params[]) return cmd_cmds(playerid,params);

CMD:stats(playerid,params[])
{
	new string[128], stats[256], id, title[64];

	if(sscanf(params, "i", id))
	{
		format(title, sizeof(title), "Stats of %s (%i)", PlayerInfo[playerid][Name], playerid);

		format(string, sizeof(string), "Name: %s (ID: %i)\nLevel: %s (%i)\nVIP: %s\nPing: %i\nIP: %s\n\n", PlayerInfo[playerid][Name], playerid, GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerLevel(playerid), IsPlayerVIP(playerid) ? ("yes") : ("no"), GetPlayerPing(playerid), PlayerInfo[playerid][IP]);
		strcat(stats, string);

		format(string, sizeof(string), "Kills: %i\nDeaths: %i\nScore: %i\nMoney: %i\n\n",  PlayerInfo[playerid][Kills],  PlayerInfo[playerid][Deaths], GetPlayerScore(playerid), GetPlayerCash(playerid));
		strcat(stats, string);

		format(string, sizeof(string), "Online time (session): %s\nOnline time (total): %s\n\n", MsToString(NetStats_GetConnectedTime(playerid),"%1h:%1m:%1s"), MsToString(((NetStats_GetConnectedTime(playerid)/1000)+PlayerInfo[playerid][OnlineTime])*1000,"%1h:%1m:%1s"));
		strcat(stats, string);
	}
	else
	{
		format(title, sizeof(title), "Stats of %s (%i)", PlayerInfo[id][Name], id);

		format(string, sizeof(string), "Name: %s (ID: %i)\nLevel: %s (%i)\nVIP: %s\nPing: %i\nIP: %s\n\n", PlayerInfo[id][Name], id, GetPlayerLevelEx(GetPlayerLevel(id)), GetPlayerLevel(id), IsPlayerVIP(id) ? ("yes") : ("no"), GetPlayerPing(id), PlayerInfo[id][IP]);
		strcat(stats, string);

		format(string, sizeof(string), "Kills: %i\nDeaths: %i\nScore: %i\nMoney: %i\n\n",  PlayerInfo[id][Kills], PlayerInfo[id][Deaths], GetPlayerScore(id), GetPlayerCash(id));
		strcat(stats, string);

		format(string, sizeof(string), "Online time (session): %s\nOnline time (total): %s\n", MsToString(NetStats_GetConnectedTime(id),"%1h:%1m:%1s"), MsToString(((NetStats_GetConnectedTime(id)/1000)+PlayerInfo[id][OnlineTime])*1000,"%1h:%1m:%1s"));
		strcat(stats, string);
	}

	ShowPlayerDialog(playerid, DIALOG_NORESPONSE, DIALOG_STYLE_MSGBOX, title, stats, "OK", "");
	return 1;
}

CMD:admins(playerid, params[])
{
	new string[128], admins[256*5], count = 0;
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(PlayerInfo[i][Adminlevel] == 0) continue;
		format(string, sizeof(string), "Name: %s - Level: %s (%i)\n", PlayerInfo[i][Name], GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerLevel(playerid));
		strcat(admins, string);

		count++;
	}

	if(count == 0)
		admins = "No admins online";
	else
	{
		format(string, sizeof(string), "There %s %i admin%s online:\n\n", count == 1 ? ("is") : ("are"), count, count != 1 ? ("s") : (""));
		strins(admins, string, 0);
	}

	ShowPlayerDialog(playerid, DIALOG_NORESPONSE, DIALOG_STYLE_MSGBOX, "Admins", admins, "OK", "");
	return 1;
}

CMD:report(playerid, params[])
{
	new id, reason[32], string[128];
	if(sscanf(params, "is[32]", id, reason)) return SendClientMessage(playerid, COLOR_RED, "Usage: /report [playerid] [reason]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "This playerid is not online.");

	format(string, sizeof(string), "Incomming report! %s (%i) about %s (%i): %s", PlayerInfo[playerid][Name], playerid, PlayerInfo[id][Name], id, reason);
	SendClientMessageToAdmins(COLOR_RED, string);

	SendClientMessage(playerid,COLOR_RED,"Your report has been sent to all online administrators.");
	return 1;
}

// ================= ADMIN COMMANDS ===========================
CMD:cpanel(playerid, params[])
{
	if(IsPlayerAdmin(playerid))
	{
		PlayerInfo[playerid][Adminlevel] = LEVEL_OWNER;
		SendClientMessage(playerid, COLOR_GREEN, "Server assigned you as an owner (level 5)");
	}
	
	ShowPlayerDialogEx(playerid, DIALOG_CPANEL);
	return 1;
}

CMD:a(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	new msg[128], string[256];
	if(sscanf(params, "s[128]", msg)) return SendClientMessage(playerid, COLOR_RED, "Usage: /a(chat) [message]");

	format(string, sizeof(string), COL_ADMIN_1"%s %s (%i):"COL_ADMIN_2" %s", GetPlayerLevelEx(GetPlayerLevel(playerid)), PlayerInfo[playerid][Name], playerid, msg);
	SendClientMessageToAdmins(-1, string);
	return 1;
}

CMD:achat(playerid, params[]) return cmd_a(playerid, params);

CMD:warn(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	return 1;
}

CMD:asay(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	return 1;
}

CMD:crew(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	return 1;
}

CMD:eject(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	return 1;
}

CMD:mute(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	return 1;
}

CMD:unmute(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	return 1;
}

CMD:weaponids(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	return 1;
}

CMD:slap(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	return 1;
}

CMD:spectate(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	return 1;
}

CMD:disarm(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	return 1;
}

CMD:adminduty(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) return 0;
	return 1;
}


CMD:announce(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	return 1;
}

CMD:kick(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	return 1;
}

CMD:jail(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	return 1;
}

CMD:unjail(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	return 1;
}

CMD:facepunch(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	return 1;
}

CMD:freeze(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	return 1;
}

CMD:unfreeze(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	return 1;
}

CMD:burn(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	return 1;
}

CMD:crash(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	return 1;
}

CMD:ajetpack(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
	return 1;
}

// CMD:veh(playerid, params[])
// {
// 	if(!IsPlayerAdminEx(playerid, LEVEL_MOD)) return 0;
// 	return 1;
// }


CMD:cage(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:uncage(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:get(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:goto(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:ban(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:setinterior(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:spawn(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:sethealth(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:setarmour(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:healall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:setskin(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:giveweapon(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:explode(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:settime(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:setweather(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:reconnect(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:setallskin(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:setalltime(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}

CMD:setallweather(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) return 0;
	return 1;
}



CMD:setscore(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	return 1;
}

CMD:setmoney(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	return 1;
}

CMD:armourall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	return 1;
}

CMD:setallscore(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	return 1;
}

CMD:spawnall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	return 1;
}

CMD:giveallweapon(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	return 1;
}

CMD:killall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	return 1;
}

CMD:getall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	return 1;
}

CMD:ejectall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	return 1;
}

CMD:godall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	return 1;
}

CMD:ungodall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	return 1;
}

CMD:freezeall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	return 1;
}

CMD:unfreezeall(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_ADMIN)) return 0;
	return 1;
}


CMD:setlevel(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_OWNER)) return 0;
	return 1;
}

CMD:fakechat(playerid, params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_OWNER)) return 0;
	return 1;
}

CMD:makevip(playerid,params[])
{
	if(!IsPlayerAdminEx(playerid, LEVEL_OWNER)) return 0;

	new id, duration, string[128];
	if(sscanf(params, "iI(0)", id, duration)) return SendClientMessage(playerid, COLOR_RED, "Usage: /makevip [playerid] [Optional: duration (in months)]");

	if(duration == 0)
	{
		mysql_format(gCon, query,sizeof(query),"INSERT INTO Vips (Name,Duration) VALUES ('%e', 0)",PlayerInfo[id][Name]);
		format(string, sizeof(string), "ever.");
	}
	else
	{
		mysql_format(gCon, query,sizeof(query),"INSERT INTO Vips (Name,Duration) VALUES ('%e', UNIX_TIMESTAMP(DATE_ADD(NOW(), INTERVAL + %d MONTH)))",PlayerInfo[id][Name],duration);
		format(string, sizeof(string), "%i months.", duration);
	}

	mysql_tquery(gCon, query);

	VipInfo[id][Duration] = duration; //as long as it's not -1 it's fine.

	format(string, sizeof(string),  COL_VIP_1"-[VIP]-"COL_VIP_2" %s has made you a VIP! You are vip for %s", PlayerInfo[playerid][Name], string);
	SendClientMessage(id, -1, string);
	return 1;
}

CMD:removevip(playerid, params[])
{
	if(GetPlayerLevel(playerid) != LEVEL_OWNER) return 0;

	new id;
	if(sscanf(params, "i", id)) return SendClientMessage(playerid, COLOR_RED, "Usage: /removevip [playerid]");
	if(!IsPlayerVIP(id)) return SendClientMessage(playerid, COLOR_RED, "This player isn't vip");

	VipInfo[id][Duration] = -1;
	mysql_format(gCon, query, sizeof(query), "DELETE FROM Vips WHERE Name = '%e'", PlayerInfo[id][Name]);
	mysql_tquery(gCon, query);

	SendClientMessage(id, -1, COL_VIP_1"-[VIP]-"COL_VIP_2" Your vip status has been removed.");
	return 1;
}

// ================= VIP COMMANDS ===========================
CMD:vipveh(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	ShowPlayerDialog(playerid,DIALOG_VIP_VEH,DIALOG_STYLE_INPUT,"Spawn a Vehicle","Please fill in your vehicle Name","Spawn","Back");
	return 1;
}

CMD:viprep(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_RED, "You're not in any vehicle.");
	RepairVehicle(GetPlayerVehicleID(playerid));
	SendClientMessage(playerid, -1, COL_VIP_1"-[VIP]- "COL_VIP_2"Your vehicle has been repaired.");
	return 1;
}

CMD:viprepair(playerid, params[]) return cmd_viprep(playerid, params);

CMD:vipnos(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_RED, "You're not in any vehicle.");
	AddVehicleComponent(GetPlayerVehicleID(playerid), 1010);
	SendClientMessage(playerid, -1, COL_VIP_1"-[VIP]- "COL_VIP_2"Nitro has been added to your vehicle.");
	return 1;
}

CMD:vipflip(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_RED, "You're not in any vehicle.");

	new Float:a;
	GetVehicleZAngle(GetPlayerVehicleID(playerid),Float:a);
	SetVehicleZAngle(GetPlayerVehicleID(playerid),Float:a);
	SendClientMessage(playerid, -1, COL_VIP_1"-[VIP]- "COL_VIP_2" Your vehicle has been flipped.");
	return 1;
}

CMD:vipunlimnos(playerid, params[])
{
	new string[128];
	if(!IsPlayerVIP(playerid)) return 0;
	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_RED, "You're not in any vehicle.");

	VipInfo[playerid][Toggle][0] = !VipInfo[playerid][Toggle][0];
	format(string, sizeof(string), COL_VIP_1"-[VIP]-"COL_VIP_2" Unlimited nitro has been turned %s", VipInfo[playerid][Toggle][0] ? ("on") : ("off"));
	SendClientMessage(playerid, -1, string);
	return 1;
}

CMD:vipheal(playerid, params[])
{
	new string[128];

	if(!IsPlayerVIP(playerid)) return 0;
	if(GetTickCount() - PlayerInfo[playerid][Tick][2] > COOLDOWN_VIP_HEAL*1000)
	{
		PlayerInfo[playerid][Tick][2] = GetTickCount();

		SetPlayerHealth(playerid, 100.0);
		SendClientMessage(playerid, -1, COL_VIP_1"-[VIP]-"COL_VIP_2" Your health has succesfully been restored.");
	}
	else
	{
		format(string, sizeof(string), "You can't do that right now. You can again in %s.",MsToString(COOLDOWN_VIP_HEAL*1000 - (GetTickCount() - PlayerInfo[playerid][Tick][2]), "%1m minute(s) and %1s second(s)"));
		SendClientMessage(playerid, COLOR_RED, string);
	}
	return 1;
}

CMD:vipgodcar(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	new string[128];

	VipInfo[playerid][Toggle][2] = !VipInfo[playerid][Toggle][2];
	format(string, sizeof(string), COL_VIP_1"-[VIP]-"COL_VIP_2"You have %s godcar", VipInfo[playerid][Toggle][2] ? ("enabled") : ("disabled"));
	SendClientMessage(playerid, -1, string);
	return 1;
}

CMD:viplock(playerid, params[])
{
	if(!IsPlayerVIP(playerid)) return 0;
	if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_RED, "You're not in any vehicle.");
	new string[128];

	if(!VipInfo[playerid][Toggle][1])
		LockVehicle(GetPlayerVehicleID(playerid));
	else
		UnlockVehicle(GetPlayerVehicleID(playerid));

	format(string, sizeof(string), COL_VIP_1"-[VIP]-"COL_VIP_2" Your vehicle has been %slocked", !VipInfo[playerid][Toggle][1] ? ("") : ("un"));
	SendClientMessage(playerid, -1, string);

	VipInfo[playerid][Toggle][1] = !VipInfo[playerid][Toggle][1];
	return 1;
}