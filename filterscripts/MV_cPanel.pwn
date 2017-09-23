#include <a_samp>
#include <a_mysql>
#include <sscanf2>
#include <zcmd>
#include <KickBan>
#include <MV_cPanel/cPanel_Config>

#define COL_RED		"{FF0000}"
#define COL_WHITE	"{FFFFFF}"
#define COL_BLUE	"{0000FF}"
#define COL_GREEN 	"{6EF83C}"

#define COLOR_RED	0xAA3333AA
#define COLOR_GREEN	0x00FF00FF

#define HOLDING(%0) 	((newkeys & (%0)) == (%0))
#define RELEASED(%0)	(((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))

new MySQL:gCon;

enum
{
	DIALOG_NORESPONSE = FREE_DIALOG_ID,
	DIALOG_REGISTER,
	DIALOG_LOGIN,
	DIALOG_VIP_VEH,
	DIALOG_CPANEL,
	DIALOG_CPANEL_PLAYERPANEL,
	DIALOG_CPANEL_SERVERPANEL,
	DIALOG_CPANEL_SERVERPANEL_0,
	DIALOG_CPANEL_SERVERPANEL_1,
	DIALOG_CPANEL_SERVERPANEL_2,
	DIALOG_CPANEL_SERVERPANEL_3,
	DIALOG_CPANEL_SERVERPANEL_4,
	DIALOG_CPANEL_SERVERPANEL_5,
	DIALOG_CPANEL_PP_LIST,
	DIALOG_CPANEL_PP_LIST_JAIL,
	DIALOG_CPANEL_PP_LIST_KICK,
	DIALOG_CPANEL_PP_LIST_BAN,
	DIALOG_CPANEL_PP_LIST_LEVEL
};

enum gPlayerInfo
{
	Name[MAX_PLAYER_NAME],
	IP[16],
	Score,
	Money,
	Adminlevel,
	OnlineTime,
	Tick[3], //0 = commands, 1 = chat, 2 = vip heal
	pTimer[4], //0 = PlayerTimer, 1 = MuteTimer, 2 = jailtimer, 3 = spectatetimer
	Selected_Id,
	Warns,
	Muted,
	OldSkin,
	Cage[4],
	bool:Reconnecting
};

new PlayerInfo[MAX_PLAYERS][gPlayerInfo], query[256];

enum gVipInfo
{
	Duration,
	bool:Toggle[3] //0 = unlim nos, 1 = lock, 2 = godcar
};

new VipInfo[MAX_PLAYERS][gVipInfo];

enum
{
	LEVEL_TRIAL_MOD = 1,
	LEVEL_MOD,
	LEVEL_TRIAL_ADMIN,
	LEVEL_ADMIN,
	LEVEL_OWNER
};

enum
{
	LOG_TYPE_CMDS,
	LOG_TYPE_ACMDS,
	LOG_TYPE_REPORTS,
	LOG_TYPE_AC
};

enum e_vInfo
{
	vName[24],
	vModel
};

new aVehicleNames[212][e_vInfo] =
{
	{"Landstalker", 400}, {"Bravura", 401},{"Buffalo", 402},{"Linerunner", 403},{"Perrenial", 404},{"Sentinel", 405},
	{"Dumper", 406},{"Firetruck", 407},{"Trashmaster", 408},{"Stretch", 409},{"Manana", 410},{"Infernus", 411},{"Voodoo", 412},
	{"Pony", 413},{"Mule", 414},{"Cheetah", 415},{"Ambulance", 416},{"Leviathan", 417},{"Moonbeam", 418},{"Esperanto", 419},
	{"Taxi", 420},{"Washington", 421},{"Bobcat", 422},{"Mr Whoopee", 423},{"BF Injection", 424},{"Hunter", 425},{"Premier", 426},
	{"Enforcer", 427},{"Securicar", 428},{"Banshee", 429},{"Predator", 430},{"Bus", 431},{"Rhino", 432},{"Barracks", 433},{"Hotknife", 434},
	{"Trailer 1", 435},	{"Previon", 436},{"Coach", 437},{"Cabbie", 438},{"Stallion", 439},{"Rumpo", 440},{"RC Bandit", 441},
	{"Romero", 442},{"Packer", 443},{"Monster", 444},{"Admiral", 445},{"Squalo", 446},{"Seasparrow", 447},{"Pizzaboy", 448},
	{"Tram", 449},{"Trailer 2", 450},{"Turismo", 451},{"Speeder", 452},{"Reefer", 453},{"Tropic", 454},{"Flatbed", 455},{"Yankee", 456},
	{"Caddy", 457},{"Solair", 458},{"Berkley's RC Van", 459},{"Skimmer", 460},{"PCJ-600", 461},{"Faggio", 462},{"Freeway", 463},
	{"RC Baron", 464},{"RC Raider", 465},{"Glendale", 466},{"Oceanic", 467},{"Sanchez", 468},{"Sparrow", 469},{"Patriot", 470},
	{"Quad", 471},{"Coastguard", 472},{"Dinghy", 473},{"Hermes", 474},{"Sabre", 475},{"Rustler", 476},{"ZR-350", 477},{"Walton", 478},
	{"Regina", 479},{"Comet", 480},{"BMX", 481},{"Burrito", 482},{"Camper", 483},{"Marquis", 484},{"Baggage", 485},{"Dozer", 486},
	{"Maverick", 487},{"News Chopper", 488},{"Rancher", 489},{"FBI Rancher", 490},{"Virgo", 491},{"Greenwood", 492},{"Jetmax", 493},
	{"Hotring", 494},{"Sandking", 495},{"Blista Compact", 496},{"Police Maverick", 497},{"Boxville", 498},{"Benson", 499},
	{"Mesa", 500},{"RC Goblin", 501},{"Hotring Racer A", 502},{"Hotring Racer B", 503},{"Bloodring Banger", 504},{"Rancher", 505},
	{"Super GT", 506},{"Elegant", 507},{"Journey", 508},{"Bike", 509},{"Mountain Bike", 510},{"Beagle", 511},{"Cropdust", 512},{"Stunt", 513},
	{"Tanker", 514},{"Roadtrain", 515},{"Nebula", 516},{"Majestic", 517},{"Buccaneer", 518},{"Shamal", 519},{"Hydra", 520},{"FCR-900", 521},
	{"NRG-500", 522},{"HPV1000", 523},{"Cement Truck", 524},{"Tow Truck", 525},{"Fortune", 526},{"Cadrona", 527},{"FBI Truck", 528},
	{"Willard", 529},{"Forklift", 530},{"Tractor", 531},{"Combine", 532},{"Feltzer", 533},{"Remington", 534},{"Slamvan", 535},{"Blade", 536},
	{"Freight", 537},{"Streak", 538},{"Vortex", 539},{"Vincent", 540},{"Bullet", 541},{"Clover", 542},{"Sadler", 543},{"Firetruck LA", 544}, //firela
	{"Hustler", 545},{"Intruder", 546},{"Primo", 547},{"Cargobob", 548},{"Tampa", 549},{"Sunrise", 550},{"Merit", 551},{"Utility", 552},
	{"Nevada", 553},{"Yosemite", 554},{"Windsor", 555},{"Monster A", 556},{"Monster B", 557},{"Uranus", 558},{"Jester", 559},{"Sultan", 560},
	{"Stratum", 561},{"Elegy", 562},{"Raindance", 563},{"RC Tiger", 564},{"Flash", 565},{"Tahoma", 566},{"Savanna", 567},{"Bandito", 568},
	{"Freight Flat", 569},{"Streak Carriage", 570},{"Kart", 571},{"Mower", 572},{"Duneride", 573},{"Sweeper", 574},{"Broadway", 575},
	{"Tornado", 576},{"AT-400", 577},{"DFT-30", 578},{"Huntley", 579},{"Stafford", 580},{"BF-581", 581},{"Newsvan", 582},{"Tug", 583},
	{"Trailer 3", 584},{"Emperor", 585},{"Wayfarer", 586},{"Euros", 587},{"Hotdog", 588},{"Club", 589},{"Freight Carriage", 590},{"Trailer 3", 591}, //artict3
	{"Andromada", 592},{"Dodo", 593},{"RC Cam", 594},{"Launch", 595},{"LSPD", 596},{"SFPD", 597},{"fLVPD", 598},{"Police Ranger", 599},{"Picador", 600},
	{"SWAT", 601},{"Alpha", 602},{"Phoenix", 603},{"Glendale", 604},{"Sadler", 605},{"Luggage Trailer A", 606},{"Luggage Trailer B", 607},
	{"Stair Trailer", 608},{"Boxville", 609},{"Farm Plow", 610},{"Utility Trailer", 611}
};

enum e_pvehicles
{
	pVehicle,
	pOwner[MAX_PLAYER_NAME],
	Text3D:pVehicleLabel
};

new bool:ChatEnabled;
new PrivateVehicles[MAX_PRIV_VEHICLES][e_pvehicles];

public OnFilterScriptInit()
{
	print("\n--------------------------------------");
	print("[MV]_cPanel by Michael@Belgium");
	print("--------------------------------------\n");

	gCon = mysql_connect(SQL_SERVER, SQL_USER, SQL_PASS, SQL_DB);

	if(mysql_errno(gCon) != 0)
	{
		printf("Could not connect to database %s on %s", SQL_DB, SQL_SERVER);
		print("Shutting down server.");

		SendRconCommand("exit");
		return 1;
	}
	
	#if LOG_MYSQL
		mysql_log(ALL);
	#endif

	ChatEnabled = true;

	for(new i = 0; i < MAX_PRIV_VEHICLES; i++)
	{
		PrivateVehicles[i][pOwner] = EOS;
		PrivateVehicles[i][pVehicle] = INVALID_VEHICLE_ID;
		PrivateVehicles[i][pVehicleLabel] = Text3D:INVALID_3DTEXT_ID;
	}
	return 1;
}

public OnFilterScriptExit()
{
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	return 1;
}

public OnPlayerConnect(playerid)
{
	GetPlayerName(playerid, PlayerInfo[playerid][Name], MAX_PLAYER_NAME);
	GetPlayerIp(playerid, PlayerInfo[playerid][IP], 16);

	PlayerInfo[playerid][Score] = 
	PlayerInfo[playerid][Money] =
	PlayerInfo[playerid][OnlineTime] = 
	PlayerInfo[playerid][Warns] =
	PlayerInfo[playerid][Muted] =
	PlayerInfo[playerid][Adminlevel] = 0;
	PlayerInfo[playerid][Selected_Id] = INVALID_PLAYER_ID;
	PlayerInfo[playerid][OldSkin] =
	VipInfo[playerid][Duration] = -1;
	PlayerInfo[playerid][Reconnecting] =
	VipInfo[playerid][Toggle][0] =
	VipInfo[playerid][Toggle][1] =
	VipInfo[playerid][Toggle][2] = false;
	PlayerInfo[playerid][pTimer][0] = SetTimerEx("PlayerTimer", 5000, true, "i", playerid);
	for(new i = 0; i < 4; i++) PlayerInfo[playerid][Cage][i] = INVALID_OBJECT_ID;

	mysql_format(gCon, query, sizeof(query), "SELECT * FROM Bans WHERE Player = '%e' OR IP = '%e'", GetPlayerNameEx(playerid), PlayerInfo[playerid][IP]);
	mysql_tquery(gCon, query, "OnBanCheck", "i", playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	if(GetPlayerState(playerid) != PLAYER_STATE_NONE)
	{
		mysql_format(gCon, query, sizeof(query), "UPDATE Players SET Score = %i, Money = %i, Adminlevel = %i, lIP = '%s', OnlineTime = OnlineTime + %i, Warnings = %i WHERE Playername = '%e'", GetPlayerScoreEx(playerid), GetPlayerCash(playerid), GetPlayerLevel(playerid), PlayerInfo[playerid][IP], NetStats_GetConnectedTime(playerid)/1000, PlayerInfo[playerid][Warns], GetPlayerNameEx(playerid));
		mysql_query(gCon, query, false);

		mysql_format(gCon, query, sizeof(query), "UPDATE Players SET Muted = %i WHERE Playername = '%e'", PlayerInfo[playerid][Muted], GetPlayerNameEx(playerid));
		mysql_query(gCon, query, false);

		if(IsPlayerVIP(playerid))
		{
			mysql_format(gCon, query, sizeof(query), "UPDATE Vips SET Toggle0 = %d, Toggle1 = %d, Toggle2 = %d WHERE Name = '%e'", VipInfo[playerid][Toggle][0], VipInfo[playerid][Toggle][1], VipInfo[playerid][Toggle][2], GetPlayerNameEx(playerid));
			mysql_query(gCon, query, false);
		}
	}

	if(IsPlayerCaged(playerid))
		UncagePlayer(playerid);

	if(PlayerInfo[playerid][Reconnecting])
	{
		new string[64];
		format(string, sizeof(string), "unbanip %s", PlayerInfo[playerid][IP]);
		SendRconCommand(string);
		//SendRconCommand("reloadbans");

		PlayerInfo[playerid][Reconnecting] = false;
	}

	KillTimer(PlayerInfo[playerid][pTimer][0]);
	KillTimer(PlayerInfo[playerid][pTimer][1]);
	KillTimer(PlayerInfo[playerid][pTimer][2]);
	KillTimer(PlayerInfo[playerid][pTimer][3]);
	return 1;
}

public OnPlayerSpawn(playerid)
{
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(IsPlayerVIP(playerid) && VipInfo[playerid][Toggle][1])
	{
		if(newstate == PLAYER_STATE_DRIVER)
		{
			SendClientMessage(playerid, -1, COL_VIP_1"-[VIP]- "COL_VIP_2"Your vehicle has been automaticly locked.");
			LockVehicle(GetPlayerVehicleID(playerid));
		}
	}

	if(newstate == PLAYER_STATE_DRIVER)
	{
		for(new i = 0; i < MAX_PRIV_VEHICLES; i++)
		{
			if(PrivateVehicles[i][pVehicle] == INVALID_VEHICLE_ID) break; //exit immediately, there won't be any after it
			if(GetPlayerVehicleID(playerid) == PrivateVehicles[i][pVehicle])
			{
				if(!strcmp(PlayerInfo[playerid][Name], PrivateVehicles[i][pOwner]))
					SendClientMessage(playerid, COLOR_GREEN, "You entered your private vehicle.");
				else
				{
					RemovePlayerFromVehicle(playerid);
					SendClientMessage(playerid, COLOR_RED, "This vehicle is a reserved vehicle.");
				}
				break;
			}
		}
	}
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	if(IsPlayerVIP(playerid) && VipInfo[playerid][Toggle][1] && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		SendClientMessage(playerid, -1,  COL_VIP_1"-[VIP]- "COL_VIP_2"This vehicle has been unlocked as you left the vehicle.");
		UnlockVehicle(vehicleid);
	}
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	new string[128];

	switch(dialogid)
	{
		case DIALOG_LOGIN:
		{
			if(!response) return ShowPlayerDialogEx(playerid,DIALOG_LOGIN);

			mysql_format(gCon, query, sizeof(query), "SELECT * FROM Players WHERE Password = SHA2('%e', 512) AND Playername = '%e'", inputtext, GetPlayerNameEx(playerid));
			mysql_tquery(gCon, query, "OnPlayerLogin", "i", playerid);
		}

		case DIALOG_REGISTER:
		{
			if(!response) return ShowPlayerDialogEx(playerid,DIALOG_REGISTER);

			mysql_format(gCon, query, sizeof(query), "INSERT INTO Players (Playername, Password, rIP, lIP) VALUES ('%e', SHA2('%e', 512), '%s', '%s')",GetPlayerNameEx(playerid), inputtext, PlayerInfo[playerid][IP], PlayerInfo[playerid][IP]);
			mysql_query(gCon, query, false);

			SendClientMessage(playerid, COLOR_GREEN, "Successfully registered.");			
		}

		case DIALOG_VIP_VEH:
		{
			if(!response) return 1;

			if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_RED, "Please be on foot to spawn a vehicle.");
			new bool:found = false;

			for(new i = 0; i < sizeof(aVehicleNames); i++)
			{
				if(!strcmp(inputtext, aVehicleNames[i][vName], true))
				{
					new Float:pos[3], vehicle = INVALID_VEHICLE_ID;
					GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

					vehicle = CreateVehicle(aVehicleNames[i][vModel], pos[0], pos[1], pos[2], 0, -1, -1, -1);
					PutPlayerInVehicle(playerid, vehicle, 0);

					format(string, sizeof(string), COL_VIP_1"-[VIP]- "COL_VIP_2"%s (%i) has spawned a %s (%i)", GetPlayerNameEx(playerid), playerid, aVehicleNames[i][vName], aVehicleNames[i][vModel]);
					SendClientMessageToAll(-1, string);

					found = true;
					break;
				}
			}

			if(!found)
				SendClientMessage(playerid, COLOR_RED, "Couldn't find vehicle.");
		}

		case DIALOG_CPANEL:
		{
			if(!response) return 1;

			switch(listitem)
			{
				case 0:	ShowPlayerDialogEx(playerid, DIALOG_CPANEL_PLAYERPANEL);
				case 1: ShowPlayerDialogEx(playerid, DIALOG_CPANEL_SERVERPANEL);
				case 2: SendRconCommand("gmx");
			}
		}

		case DIALOG_CPANEL_PLAYERPANEL:
		{
			if(!response) return ShowPlayerDialogEx(playerid, DIALOG_CPANEL);
			if(!IsNumeric(inputtext) || isnull(inputtext)) 
			{
				SendClientMessage(playerid, COLOR_RED, "Please enter a playerid.");
				return ShowPlayerDialogEx(playerid, DIALOG_CPANEL_PLAYERPANEL);
			}

			new id = strval(inputtext);

			if (!IsPlayerConnected(id))
			{
				SendClientMessage(playerid, COLOR_RED, "This user isn't online");	
				return ShowPlayerDialogEx(playerid, DIALOG_CPANEL_PLAYERPANEL);
			}

			PlayerInfo[playerid][Selected_Id] = strval(inputtext);

			ShowPlayerDialogEx(playerid, DIALOG_CPANEL_PP_LIST);
		}

		case DIALOG_CPANEL_PP_LIST:
		{
			if(!response) return ShowPlayerDialogEx(playerid, DIALOG_CPANEL_PLAYERPANEL);
			if(PlayerInfo[playerid][Selected_Id] == INVALID_PLAYER_ID) return 1;

			new id[4];
			valstr(id, PlayerInfo[playerid][Selected_Id]);

			switch(listitem)
			{
				case 0: cmd_spectate(playerid, id);
				case 1: cmd_get(playerid, id);
				case 2: cmd_goto(playerid, id);
				case 3: 
				{
					if(IsPlayerMuted(PlayerInfo[playerid][Selected_Id]))
						cmd_unmute(playerid, id);
					else
						cmd_mute(playerid, id);
				}
				case 4: cmd_freeze(playerid, id);
				case 5: cmd_unfreeze(playerid, id);
				case 6: ShowPlayerDialog(playerid, DIALOG_CPANEL_PP_LIST_JAIL, DIALOG_STYLE_INPUT, "Jail player", "For which reason does the player need to be jailed?", "Jail", "Cancel");
				case 7: cmd_unjail(playerid, id);
				case 8: cmd_akill(playerid, id);
				case 9: ShowPlayerDialog(playerid, DIALOG_CPANEL_PP_LIST_KICK, DIALOG_STYLE_INPUT, "Kick player", "For which reason does the player need to be kicked?", "Kick", "Cancel");
				case 10: ShowPlayerDialog(playerid, DIALOG_CPANEL_PP_LIST_BAN, DIALOG_STYLE_INPUT, "Ban player", "For which reason does the player need to be banned?", "Ban", "Cancel");
				case 11: ShowPlayerDialogEx(playerid, DIALOG_CPANEL_PP_LIST_LEVEL);
			}

			if(listitem != 6 && listitem != 9 && listitem != 10 && listitem != 11)
				PlayerInfo[playerid][Selected_Id] = INVALID_PLAYER_ID;
		}

		case DIALOG_CPANEL_PP_LIST_JAIL:
		{
			if(!response) return ShowPlayerDialogEx(playerid, DIALOG_CPANEL_PP_LIST);
			format(string, sizeof(string), "%i %s", PlayerInfo[playerid][Selected_Id], inputtext);
			cmd_jail(playerid, string);

			PlayerInfo[playerid][Selected_Id] = INVALID_PLAYER_ID;
		}

		case DIALOG_CPANEL_PP_LIST_KICK:
		{
			if(!response) return ShowPlayerDialogEx(playerid, DIALOG_CPANEL_PP_LIST);
			format(string, sizeof(string), "%i %s", PlayerInfo[playerid][Selected_Id], inputtext);
			cmd_kick(playerid, string);

			PlayerInfo[playerid][Selected_Id] = INVALID_PLAYER_ID;
		}

		case DIALOG_CPANEL_PP_LIST_BAN:
		{
			if(!response) return ShowPlayerDialogEx(playerid, DIALOG_CPANEL_PP_LIST);
			format(string, sizeof(string), "%i %s", PlayerInfo[playerid][Selected_Id], inputtext);
			cmd_ban(playerid, string);

			PlayerInfo[playerid][Selected_Id] = INVALID_PLAYER_ID;
		}

		case DIALOG_CPANEL_PP_LIST_LEVEL:
		{
			if(!response) return ShowPlayerDialogEx(playerid, DIALOG_CPANEL_PP_LIST);
			if(!IsNumeric(inputtext)) 
			{
				SendClientMessage(playerid, COLOR_RED, "This isn't a number.");
				return ShowPlayerDialogEx(playerid, DIALOG_CPANEL_PP_LIST_LEVEL);
			}

			new level = strval(inputtext);
			format(string, sizeof(string), "%i %i", PlayerInfo[playerid][Selected_Id], level);
			cmd_setlevel(playerid, string);

			PlayerInfo[playerid][Selected_Id] = INVALID_PLAYER_ID;
		}

		case DIALOG_CPANEL_SERVERPANEL:
		{
			if(!response) return ShowPlayerDialogEx(playerid, DIALOG_CPANEL);

			switch(listitem)
			{
        		case 0:	ShowPlayerDialog(playerid,DIALOG_CPANEL_SERVERPANEL_0,DIALOG_STYLE_INPUT, "Change gamemode","Please fill in your gamemode name.","OK","Cancel");
        		case 1:	ShowPlayerDialog(playerid,DIALOG_CPANEL_SERVERPANEL_1,DIALOG_STYLE_INPUT, "Load filterscript","Please fill in your filterscript name.","OK","Cancel");
        		case 2:	ShowPlayerDialog(playerid,DIALOG_CPANEL_SERVERPANEL_2,DIALOG_STYLE_INPUT, "Unload filterscript","Please fill in your filterscript name.","OK","Cancel");
        		case 3:	ShowPlayerDialog(playerid,DIALOG_CPANEL_SERVERPANEL_3,DIALOG_STYLE_INPUT, "Set hostname","Please fill in your hostname.","OK","Cancel");
        		case 4:	ShowPlayerDialog(playerid,DIALOG_CPANEL_SERVERPANEL_4,DIALOG_STYLE_INPUT, "Lock server","Please fill in your server password.","OK","Cancel");
        		case 5:	ShowPlayerDialog(playerid,DIALOG_CPANEL_SERVERPANEL_5,DIALOG_STYLE_INPUT, "Set gamemodetext","Please fill in your gamemode text.","OK","Cancel");
			}
		}

		case DIALOG_CPANEL_SERVERPANEL_0:
		{
			if(!response) return ShowPlayerDialogEx(playerid, DIALOG_CPANEL_SERVERPANEL);

			format(string,sizeof(string),"changemode %s",inputtext);
			SendRconCommand(string);
		}

		case DIALOG_CPANEL_SERVERPANEL_1:
		{
			if(!response) return ShowPlayerDialogEx(playerid, DIALOG_CPANEL_SERVERPANEL);

			format(string,sizeof(string),"loadfs %s",inputtext);
			SendRconCommand(string);
		}

		case DIALOG_CPANEL_SERVERPANEL_2:
		{
			if(!response) return ShowPlayerDialogEx(playerid, DIALOG_CPANEL_SERVERPANEL);

			format(string,sizeof(string),"unloadfs %s",inputtext);
			SendRconCommand(string);
		}

		case DIALOG_CPANEL_SERVERPANEL_3:
		{
			if(!response) return ShowPlayerDialogEx(playerid, DIALOG_CPANEL_SERVERPANEL);

			format(string,sizeof(string),"hostname %s",inputtext);
			SendRconCommand(string);

			format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2" changed hostname to '%s'.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), inputtext);
			SendClientMessageToAdmins(-1, string);
		}

		case DIALOG_CPANEL_SERVERPANEL_4:
		{
			if(!response) return ShowPlayerDialogEx(playerid, DIALOG_CPANEL_SERVERPANEL);

			format(string,sizeof(string),"password %s",inputtext);
			SendRconCommand(string);

			format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2" changed the password of the server.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid));
			SendClientMessageToAdmins(-1, string);
		}

		case DIALOG_CPANEL_SERVERPANEL_5:
		{
			if(!response) return ShowPlayerDialogEx(playerid, DIALOG_CPANEL_SERVERPANEL);

			SetGameModeText(inputtext);

			format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2" changed the gamemodetext to '%s'.", GetPlayerLevelEx(GetPlayerLevel(playerid)), GetPlayerNameEx(playerid), inputtext);
			SendClientMessageToAdmins(-1, string);
		}
	}
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	if(IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD))
	{
		PlayerInfo[playerid][Selected_Id] = clickedplayerid;

		ShowPlayerDialogEx(playerid, DIALOG_CPANEL_PP_LIST);
	}
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(IsPlayerVIP(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER && VipInfo[playerid][Toggle][0])
	{
		if (HOLDING(KEY_FIRE))	AddVehicleComponent(GetPlayerVehicleID(playerid), 1010);
	    if (RELEASED(KEY_FIRE))	RemoveVehicleComponent(GetPlayerVehicleID(playerid), 1010);
	}
	return 1;
}

public OnPlayerCommandReceived(playerid, cmdtext[])
{
	#if MUTE_EQUALS_NOCMDS
		if(IsPlayerMuted(playerid))
		{
			new string[128];
			format(string, sizeof(string), "You are muted for %i minute%s", PlayerInfo[playerid][Muted], PlayerInfo[playerid][Muted] != 1 ? ("s") : (""));
			SendClientMessage(playerid, COLOR_RED, string);
			return 0;
		}
	#endif

	if(GetTickCount() - PlayerInfo[playerid][Tick][0] > COOLDOWN_COMMAND*1000)
		PlayerInfo[playerid][Tick][0] = GetTickCount();
	else
	{
		SendClientMessage(playerid, COLOR_RED, "Slow down at executing commands.");
		return 0;
	}
	return 1;
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
	#if LOG_COMMANDS
	if(success)
		SaveLog(LOG_TYPE_CMDS, GetPlayerNameEx(playerid), cmdtext);
	#endif

	if(!success)
		SendClientMessage(playerid, COLOR_RED, "Unknown command. Check /cmds to view all available commands.");
	return 1;
}

public OnPlayerText(playerid, text[])
{
	new string[128];
	if(IsPlayerMuted(playerid))
	{
		format(string, sizeof(string), "You are muted for %i minute%s", PlayerInfo[playerid][Muted], PlayerInfo[playerid][Muted] != 1 ? ("s") : (""));
		SendClientMessage(playerid, COLOR_RED, string);
		return 0;
	}

	if(!ChatEnabled)
	{
		SendClientMessage(playerid, COLOR_RED, "The chat is disabled.");
		return 0;
	}

	if(IsAdvertisement(text))
	{
		SendClientMessage(playerid, COLOR_RED, "Possible advertisement.");

		format(string, sizeof(string), COL_ADMIN_1"-[%s]- "COL_ADMIN_2"possible advertisement detected from %s (%i): %s - Message prevent to sent.", AC_NAME, GetPlayerNameEx(playerid), playerid, text);
		SendClientMessageToAdmins(-1, string);
		#if LOG_ANTICHEAT
			SaveLog(LOG_TYPE_AC, "", "ADVERT", GetPlayerNameEx(playerid), text);
		#endif
		return 0;
	}

	if(GetTickCount() - PlayerInfo[playerid][Tick][1] > COOLDOWN_CHAT*1000)
		PlayerInfo[playerid][Tick][1] = GetTickCount();
	else
	{
		SendClientMessage(playerid, COLOR_RED, "Slow down at chatting.");
		return 0;
	}
	return 1;
}

#include <MV_cPanel/cPanel_Functions>
#include <MV_cPanel/cPanel_Callbacks>
#include <MV_cPanel/cPanel_Commands>