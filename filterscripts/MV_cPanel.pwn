#include <a_samp>
#include <a_mysql>
#include <sscanf2>
#include <zcmd>

#define SQL_SERVER	"127.0.0.1"
#define SQL_USER	""
#define SQL_PASS	""
#define SQL_DB		""

#define COL_RED		"{FF0000}"
#define COL_WHITE	"{FFFFFF}"
#define COL_BLUE	"{0000FF}"
#define COL_VIP_1	"{411053}"
#define COL_VIP_2	"{7B498D}"
#define COL_GREEN 	"{6EF83C}"
#define COL_ADMIN_1	"{FF0A00}"
#define COL_ADMIN_2	"{FFFFFF}"

#define HOLDING(%0) 	((newkeys & (%0)) == (%0))
#define RELEASED(%0)	(((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))

#define COLOR_RED	0xAA3333AA
#define COLOR_GREEN	0x00FF00FF

#define COOLDOWN_COMMAND	5	//time in seconds between command executes
#define COOLDOWN_CHAT		2	//time in seconds between chat messages
#define COOLDOWN_VIP_HEAL	600	//time in seconds between vip healing (/vipheal)

new MySQL:gCon;

enum
{
	DIALOG_NORESPONSE,
	DIALOG_REGISTER,
	DIALOG_LOGIN,
	DIALOG_VIP_VEH
};

enum gPlayerInfo
{
	Name[MAX_PLAYER_NAME],
	IP[16],
	Score,
	Money,
	Adminlevel,
	Deaths,
	Kills,
	OnlineTime,
	Tick[3], //0 = commands, 1 = chat, 2 = vip heal
	pTimer
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
	}
	else
		mysql_log(ALL);

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
	PlayerInfo[playerid][Kills] =
	PlayerInfo[playerid][Deaths] =
	PlayerInfo[playerid][OnlineTime] = 
	PlayerInfo[playerid][Adminlevel] = 0;
	VipInfo[playerid][Duration] = -1;
	VipInfo[playerid][Toggle][0] =
	VipInfo[playerid][Toggle][1] =
	VipInfo[playerid][Toggle][2] = false;
	PlayerInfo[playerid][pTimer] = SetTimerEx("PlayerTimer", 5000, true, "i", playerid);

	mysql_format(gCon, query, sizeof(query), "SELECT Playername FROM Players WHERE Playername = '%e'", PlayerInfo[playerid][Name]);
	mysql_tquery(gCon, query, "OnAccountCheck", "i", playerid);

	mysql_format(gCon, query, sizeof(query), "SELECT * FROM Vips WHERE Name = '%e'",  PlayerInfo[playerid][Name]);
	mysql_tquery(gCon, query, "OnVipLoaded", "i", playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	if(GetPlayerState(playerid) != PLAYER_STATE_NONE)
	{
		mysql_format(gCon, query, sizeof(query), "UPDATE Players SET Score = %i, Money = %i, Adminlevel = %i, Kills = %i, Deaths = %i, lIP = '%s', OnlineTime = OnlineTime + %i WHERE Playername = '%e'", PlayerInfo[playerid][Score], GetPlayerCash(playerid), PlayerInfo[playerid][Adminlevel], PlayerInfo[playerid][Kills], PlayerInfo[playerid][Deaths], PlayerInfo[playerid][IP], NetStats_GetConnectedTime(playerid)/1000,  PlayerInfo[playerid][Name]);
		mysql_query(gCon, query, false);

		mysql_format(gCon, query, sizeof(query), "UPDATE Vips SET Toggle0 = %d, Toggle1 = %d, Toggle2 = %d WHERE Name = '%e'", VipInfo[playerid][Toggle][0], VipInfo[playerid][Toggle][1], VipInfo[playerid][Toggle][2], PlayerInfo[playerid][Name]);
		mysql_query(gCon, query, false);
	}

	KillTimer(PlayerInfo[playerid][pTimer]);
	return 1;
}

public OnPlayerSpawn(playerid)
{
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	if(killerid != INVALID_PLAYER_ID)
	{
		PlayerInfo[killerid][Kills]++;
		PlayerInfo[playerid][Deaths]++;
	}
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
		case DIALOG_LOGIN:
		{
			if(!response) 
				ShowPlayerDialogEx(playerid,DIALOG_LOGIN);
			else
			{
				mysql_format(gCon, query, sizeof(query), "SELECT * FROM Players WHERE Password = SHA2('%e', 512) AND Playername = '%e'", inputtext, PlayerInfo[playerid][Name]);
				mysql_tquery(gCon, query, "OnPlayerLogin", "i", playerid);
			}
		}

		case DIALOG_REGISTER:
		{
			if(!response)
				ShowPlayerDialogEx(playerid,DIALOG_REGISTER);
			else
			{
				mysql_format(gCon, query, sizeof(query), "INSERT INTO Players (Playername, Password, rIP, lIP) VALUES ('%e', SHA2('%e', 512), '%s', '%s')",PlayerInfo[playerid][Name], inputtext, PlayerInfo[playerid][IP], PlayerInfo[playerid][IP]);
				mysql_query(gCon, query, false);

				SendClientMessage(playerid, COLOR_GREEN, "Successfully registered.");
			}
		}

		case DIALOG_VIP_VEH:
		{
			if(!response) return 1;

			if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_RED, "Please be on foot to spawn a vehicle.");
			new bool:found = false, string[128];

			for(new i = 0; i < sizeof(aVehicleNames); i++)
			{
				if(!strcmp(inputtext, aVehicleNames[i][vName], true))
				{
					new Float:pos[3], vehicle = INVALID_VEHICLE_ID;
					GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

					vehicle = CreateVehicle(aVehicleNames[i][vModel], pos[0], pos[1], pos[2], 0, -1, -1, -1);
					PutPlayerInVehicle(playerid, vehicle, 0);

					format(string, sizeof(string), COL_VIP_1"-[VIP]- "COL_VIP_2"%s (%i) has spawned a %s (%i)", PlayerInfo[playerid][Name], playerid, aVehicleNames[i][vName], aVehicleNames[i][vModel]);
					SendClientMessageToAll(-1, string);

					found = true;
					break;
				}
			}

			if(!found)
				SendClientMessage(playerid, COLOR_RED, "Couldn't find vehicle.");
		}
	}
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
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
	if(GetTickCount() - PlayerInfo[playerid][Tick][0] > COOLDOWN_COMMAND*1000)
		PlayerInfo[playerid][Tick][0] = GetTickCount();
	else
	{
		SendClientMessage(playerid, COLOR_RED, "Slow down at executing commands.");
		return 0;
	}
	return 1;
}

public OnPlayerText(playerid, text[])
{
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