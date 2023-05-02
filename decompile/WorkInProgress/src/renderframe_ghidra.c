#include <common.h>

void MainFrame_RenderFrame(struct GameTracker *gGT)
{
	u_int *puVar1;
	u_char bVar2;
	struct TileView *psVar3;
	char cVar4;
	u_int uVar5;
	u_int uVar6;
	char *pcVar7;
	struct QuadBlock *psVar8;
	void *pvVar9;
	u_int *puVar10;
	short sVar10;
	int iVar11;
	u_int uVar12;
	int **ppiVar13;
	struct Thread *psVar14;
	DISPENV *env;
	short startX;
	struct TileView *psVar15;
	short startY;
	short sVar16;
	int iVar17;
	struct GameTracker *psVar18;
	int iVar23;
	int iVar19;
	int iVar20;
	int iVar21;
	struct GameTracker *psVar22;
	struct Level *level;
	struct mesh_info *playstationVar23;
	void *in_stack_ffffffb0;
	RECT local_40;
	short local_38;
	short local_36;
	RECT local_30;
	char bool_menubox_mainMenuState;
	
	playstationVar23 = (struct mesh_info *)0;
	level = gGT->level1;
	bool_menubox_mainMenuState = false;
	uVar5 = LOAD_IsOpen_MainMenu();
	if ((uVar5 != 0) && (sdata->ptrActiveMenuBox == &OVR_230.menubox_mainMenu))
	{
		bool_menubox_mainMenuState = (OVR_230.menubox_mainMenu.state & 0x10) == 0;
	}
	if
	(
		(
			(
				((sdata->gGT->gameMode1 & 0x20000000) == 0) &&
				(uVar6 = MainFrame_HaveAllPads((u_short)(u_char)sdata->gGT->numPlyrNextGame), 
				(uVar6 & 0xffff) == 0)
			) && (sdata->gGT->boolDemoMode == 0)
		) &&
		(!bool_menubox_mainMenuState)
	)
	{
		sVar16 = data_errorPosY[sdata->errorMessagePosIndex];
		local_40.h = 0xe;
		if
		(
			(sdata->gGamepads->slotBuffer[0].meta[1] == -0x80) ||
			(sVar10 = 0,  2 < sdata->gGT->numPlyrNextGame)
		)
		{
			sVar10 = 2;
		}
		iVar23 = 0;
		if (sdata->gGT->numPlyrNextGame != 0)
		{
			iVar20 = 0;
			local_40.h = 0xe;
			do
			{
				pcVar7 = *(char **)(sdata->gGamepads->gamepad[0].data18 + iVar20 + -0xc);
				if ((pcVar7 == (char *)0) || (*pcVar7 != 0))
				{
					local_40.h = local_40.h + 8;
					DecalFont_DrawLine(sdata->lngStrings[data.lngIndex_gamepadUnplugged[iVar23 + sVar10]], 0x100, sVar16, FONT_SMALL, (CENTER_TEXT | ORANGE));
					sVar16 = sVar16 + 8;
				}
				iVar23 = iVar23 + 1;
				iVar20 = iVar20 + 0x50;
			} while (iVar23 < sdata->gGT->numPlyrNextGame);
		}
		DecalFont_DrawLine(sdata->lngStrings[0x2b], 0x100, sVar16, FONT_SMALL, (CENTER_TEXT | ORANGE));
		local_40.x = -0x14;
		local_40.w = 0x228;
		local_40.y = data_errorPosY[sdata->errorMessagePosIndex] + -3;
		MenuBox_DrawInnerRect(&local_40, 1, (u_long *)(sdata->gGT->backBuffer->otMem).startPlusFour);
	}
	iVar17 = 0;
	iVar19 = 0;
	iVar21 = 0x168;
	iVar20 = iVar19;
	do
	{
		if (*(u_short *)((int)sdata->finalLapTextTimer + iVar17) != 0)
		{
			iVar11 = (int)((0x5a - (u_int)*(u_short *)((int)sdata->finalLapTextTimer + iVar17)) * 0x10000) >> 0x10;
			if (iVar11 < 0x51)
			{
				if (iVar11 < 0xb)
				{
					sVar16 = *(short *)((int)sdata->gGT->db[0].drawEnv.ofs + iVar21);
					startX = sVar16 + 100;
					startY = *(short *)((int)sdata->gGT->db[0].drawEnv.ofs + iVar21 + 2) >> 2;
					sVar16 = sVar16 >> 1;
				}
				else
				{
					iVar11 = iVar11 + -10;
					startX = *(short *)((int)sdata->gGT->db[0].drawEnv.ofs + iVar21) >> 1;
					startY = *(short *)((int)sdata->gGT->db[0].drawEnv.ofs + iVar21 + 2) >> 2;
					sVar16 = startX;
				}
			}
			else
			{
				iVar11 = iVar11 + -0x50;
				startX = *(short *)((int)sdata->gGT->db[0].drawEnv.ofs + iVar21) >> 1;
				startY = *(short *)((int)sdata->gGT->db[0].drawEnv.ofs + iVar21 + 2) >> 2;
				sVar16 = -100;
			}
			in_stack_ffffffb0 = (void *)0xa;
			InterpolatePosition2D_Linear(&local_38, startX, startY, sVar16, startY, iVar11, 10);
			DecalFont_DrawLineOT(sdata->lngStrings[0x233], (int)local_38, (int)local_36, FONT_BIG, (CENTER_TEXT | ORANGE), *(u_long **)(sdata->gGT->tileView[0].filler2 + iVar20 + -4));
			*(short *)((int)sdata->finalLapTextTimer + iVar17) = *(short *)((int)sdata->finalLapTextTimer + iVar17) + -1;
		}
		iVar17 = iVar17 + 2;
		iVar20 = iVar20 + 0x110;
		iVar19 = iVar19 + 1;
		iVar21 = iVar21 + 0x110;
	} while (iVar19 < 4);
	ElimBG_HandleState(sdata->gGT);
	if (level != 0)
	{
		playstationVar23 = level->ptr_mesh_info;
	}
	if ((gGT->renderFlags & 0x21) != 0)
	{
		MainFrame_VisMemFullFrame(gGT, level);
	}
	if ((((gGT->renderFlags & 1) != 0) && (sdata->gGT->visMem1 != 0)) && (level != 0))
	{
		CTR_CycleTex_LEV(level->ptr_anim_tex, gGT->timer);
	}
	if ((sdata->ptrActiveMenuBox != 0) || ((sdata->gGT->gameMode1 & 0x200000) != 0))
	{
		MenuBox_CollectInput();
	}
	if ((sdata->ptrActiveMenuBox != 0) && (sdata->Loading_stage == 0xffffffff))
	{
		MenuBox_ProcessState();
	}
	iVar20 = 0;
	if (gGT->numPlyrCurrGame != 0)
	{
		iVar17 = 0x168;
		psVar18 = gGT;
		psVar22 = gGT;
		do
		{
			Camera110_UpdateFrustum((struct TileView *)((int)gGT->db[0].drawEnv.ofs + iVar17 + -0x20));
			psVar8 = psVar18->cameraDC[0].ptrQuadblock;
			if (psVar8 != (struct QuadBlock *)0)
			{
				uVar6 = (u_int)(u_char)psVar8->weather_intensity;
				uVar12 = (u_int)(u_char)gGT->numPlyrCurrGame;
				/*
				if (uVar12 == 0)
				{
					trap(0x1c00);
				}
				if ((uVar12 == 0xffffffff) && (uVar6 == 0x20000000))
				{
					trap(0x1800);
				}
				*/
				psVar22->rainBuffer[0].numParticles_max = (short)((uVar6 << 2) / uVar12);
				uVar6 = (u_int)(u_char)(psVar18->cameraDC[0].ptrQuadblock)->weather_vanishRate;
				uVar12 = (u_int)(u_char)gGT->numPlyrCurrGame;
				/*
				if (uVar12 == 0)
				{
					trap(0x1c00);
				}
				if ((uVar12 == 0xffffffff) && (uVar6 == 0x20000000))
				{
					trap(0x1800);
				}
				*/
				psVar22->rainBuffer[0].vanishRate = (short)((uVar6 << 2) / uVar12);
			}
			psVar22 = (struct GameTracker *)&psVar22->db[0].drawEnv.isbg;
			psVar18 = (struct GameTracker *)psVar18->db[1].drawEnv.dr_env.code;
			iVar20 = iVar20 + 1;
			iVar17 = iVar17 + 0x110;
		} while (iVar20 < (int)(u_int)(u_char)gGT->numPlyrCurrGame);
	}
	EffectSfxRain_MakeSound(gGT);
	uVar12 = (int)sdata->frameCounter << 7;
	uVar6 = trigTable[uVar12 & 0x3ff];
	if ((uVar12 & 0x400) == 0)
	{
		uVar6 = uVar6 << 0x10;
	}
	iVar20 = (int)uVar6 >> 0x10;
	if ((uVar12 & 0x800) != 0)
	{
		iVar20 = -iVar20;
	}
	iVar20 = (iVar20 << 6) >> 0xc;
	sdata->menuRowHighlight_Normal = (iVar20 + 0x40) * 0x100 | 0x80;
	sdata->menuRowHighlight_Green = (iVar20 + 0xa0) * 0x100 | 0x400040;
	if (((gGT->renderFlags & 2) != 0) && ((u_char)sdata->gGT->numPlyrCurrGame < 2))
	{
		RenderWeather(sdata->gGT->tileView, &sdata->gGT->backBuffer->primMem, sdata->gGT->rainBuffer, sdata->gGT->numPlyrCurrGame, sdata->gGT->gameMode1 & (PAUSE_1 | PAUSE_2 | PAUSE_3 | PAUSE_4));
	}
	if (((gGT->renderFlags & 4) != 0) && (iVar20 = 0,  0 < (int)sdata->gGT->numWinners))
	{
		iVar17 = 0;
		do
		{
			DrawConfetti(sdata->gGT->tileView + *(int *)((int)sdata->gGT->winnerIndex + iVar17), &sdata->gGT->backBuffer->primMem, &sdata->gGT->confetti, sdata->gGT->frameTimer_Confetti, sdata->gGT->gameMode1 & (PAUSE_1 | PAUSE_2 | PAUSE_3 | PAUSE_4));
			iVar20 = iVar20 + 1;
			iVar17 = iVar20 * 4;
		} while (iVar20 < (int)sdata->gGT->numWinners);
	}
	if (((gGT->renderFlags & 8) != 0) && (*(short *)&(sdata->gGT->stars).numStars != 0))
	{
		RenderStars(sdata->gGT->tileView, &sdata->gGT->backBuffer->primMem, &sdata->gGT->stars, sdata->gGT->numPlyrCurrGame);
	}
	if (((gGT->renderFlags & 0x100) != 0) && (1 < (u_char)sdata->gGT->numPlyrCurrGame))
	{
		DecalMP_01(gGT);
	}
	if ((((sdata->gGT->hudFlags & 8) == 0) || (sdata->gGT->numPlyrCurrGame != 1)) || ((sdata->gGT->gameMode1 & 0x40) == 0))
	{
		bVar2 = sdata->gGT->hudFlags;
		if ((bVar2 & 1) == 0)
		{
			if ((bVar2 & 4) != 0)
			{
				CupStandings_InputAndDraw();
			}
		}
		else
		{
			uVar6 = sdata->gGT->gameMode1;
			if ((uVar6 & 0x100000) == 0)
			{
				if (((uVar6 & 0x200000) == 0) || (sdata->gGT->timerEndOfRaceVS != 0))
				{
					if ((uVar6 & 0x8000000) == 0)
					{
						DrawHUD_Racing();
					}
					else
					{
						DrawHUD_CrystalChallenge();
					}
				}
				else if (1 < sdata->Loading_stage + 5)
				{
					if ((uVar6 & 0x8000000) == 0)
					{
						if ((uVar6 & 0x20000) == 0)
						{
							if ((uVar6 & 0x400000) == 0)
							{
								if ((uVar6 & 0x4000000) == 0)
								{
									if ((uVar6 & 0x80000) != 0) goto LAB_800367d4;
									if ((sdata->gGT->gameMode2 & 0x10) == 0)
									{
										VB_EndEvent_DrawMenu();
									}
									else
									{
										sdata->gGT->hudFlags = bVar2 & 0xfe;
										sdata->gGT->hudFlags = sdata->gGT->hudFlags | 4;
									}
								}
								else
								{
									RR_EndEvent_DrawMenu();
								}
							}
							else
							{
LAB_800367d4:
								AA_EndEvent_DrawMenu();
							}
						}
						else
						{
							TT_EndEvent_DrawMenu();
						}
					}
					else
					{
						CC_EndEvent_DrawMenu();
					}
				}
			}
			else
			{
				if
				(
					(1 < sdata->gGT->overlayTransition) &&
					(
						sdata->gGT->overlayTransition--, 
						sdata->gGT->overlayTransition == 1
					)
				)
				{
					LOAD_OvrThreads(2);
				}
				uVar5 = LOAD_IsOpen_AdvHub();
				if (uVar5 == 0)
				{
					if (0xfff < (sdata->gGT->tileView_UI).fadeFromBlack_currentValue)
					{
						DrawHUD_AdvStrings();
					}
				}
				else
				{
					if ((0xfff < (sdata->gGT->tileView_UI).fadeFromBlack_currentValue) && (AH_Map_Main(),  sdata->AkuHint_RequestedHint != -1))
					{
						AH_MaskHint_Start(sdata->AkuHint_RequestedHint, sdata->AkuHint_boolInterruptWarppad);
						sdata->AkuHint_RequestedHint = -1;
						sdata->AkuHint_boolInterruptWarppad = 0;
					}
					if (sdata->gGT->overlayTransition != 0)
					{
						INSTANCE_LevRestartLInBs(gGT->level1->ptrInstDefs, gGT->level1->numInstances);
						psVar18 = sdata->gGT;
						puVar1 = &sdata->gGT->gameMode2;
						sdata->gGT->overlayTransition = 0;
						psVar22 = sdata->gGT;
						psVar18->gameMode2 = *puVar1 & 0xfffffeff;
						(psVar22->tileView_UI).fadeFromBlack_desiredResult = 0x1000;
						(psVar22->tileView_UI).fade_step = 0x2aa;
					}
				}
			}
		}
	}
	else
	{
		DrawIntroRaceText_1P();
	}
	if (((gGT->renderFlags & 0x10) != 0) && ((u_char)sdata->gGT->numPlyrCurrGame < 3))
	{
		RedBeaker_RenderRain(gGT->tileView, &gGT->backBuffer->primMem, &(sdata->gGT->JitPools).rain, sdata->gGT->numPlyrCurrGame, sdata->gGT->gameMode1 & (PAUSE_1 | PAUSE_2 | PAUSE_3 | PAUSE_4));
	}
	uVar5 = LOAD_IsOpen_Podiums();
	if ((uVar5 != 0) && (sdata->gGT->levelID == 0x29))
	{
		CS_BoxScene_InstanceSplitLines();
	}
	if ((gGT->renderFlags & 0x20) != 0)
	{
		uVar6 = (u_char)sdata->gGT->numPlyrCurrGame - 1;
		if ((sdata->gGT->gameMode1 & 0x4000000) != 0)
		{
			uVar6 = uVar6 | 4;
		}
		pvVar9 = RenderBucket_QueueLevInstances(gGT->cameraDC, (u_long *)&sdata->gGT->backBuffer->otMem, sdata->gGT->ptrRenderBucketInstance, (char *)(u_int)(u_char)sdata->LOD[uVar6], sdata->gGT->numPlyrCurrGame, sdata->gGT->gameMode1 & (PAUSE_1 | PAUSE_2 | PAUSE_3 | PAUSE_4));
		uVar6 = (u_char)sdata->gGT->numPlyrCurrGame - 1;
		if ((sdata->gGT->gameMode1 & 0x4000000) != 0)
		{
			uVar6 = uVar6 | 4;
		}
		puVar10 = (u_int *)RenderBucket_QueueNonLevInstances((sdata->gGT->JitPools).instance.taken.first, (u_long *)&sdata->gGT->backBuffer->otMem, pvVar9, (char *)(u_int)(u_char)sdata->LOD[uVar6], sdata->gGT->numPlyrCurrGame, sdata->gGT->gameMode1 & (PAUSE_1 | PAUSE_2 | PAUSE_3 | PAUSE_4));
		*puVar10 = 0;
	}
	if ((gGT->renderFlags & 0x200) != 0)
	{
		cVar4 = gGT->numPlyrCurrGame;
		iVar20 = 0;
		if (cVar4 == 0) goto code_r0x800369d8;
		iVar17 = 0x168;
		do
		{
			iVar19 = iVar17 + -0x20;
			iVar17 = iVar17 + 0x110;
			Particle_RenderList((struct TileView *)((int)gGT->db[0].drawEnv.ofs + iVar19), sdata->gGT->particleList_ordinary);
			iVar20 = iVar20 + 1;
		} while (iVar20 < (int)(u_int)(u_char)gGT->numPlyrCurrGame);
	}
	cVar4 = gGT->numPlyrCurrGame;
code_r0x800369d8:
	iVar20 = 0;
	if (cVar4 != 0)
	{
		psVar15 = gGT->tileView;
		psVar18 = gGT;
		iVar17 = 0x168;
		do
		{
			psVar3 = psVar18->tileView;
			psVar18 = (struct GameTracker *)(psVar18->db[1].drawEnv.dr_env.code + 0xd);
			Camera110_SetDrawEnv_Normal((u_long *)((int)psVar3->ptrOT + 0xffc), psVar15, gGT->backBuffer, (short *)0, 0);
			iVar20 = iVar20 + 1;
			psVar15 = (struct TileView *)((int)gGT->tileView[0].pos + iVar17 + -0x58);
			iVar17 = iVar17 + 0x110;
		} while (iVar20 < (int)(u_int)(u_char)gGT->numPlyrCurrGame);
	}
	if (((gGT->renderFlags & 0x100) != 0) && (1 < (u_char)sdata->gGT->numPlyrCurrGame))
	{
		DecalMP_02(gGT);
	}
	if ((gGT->renderFlags & 0x40) != 0)
	{
		uVar5 = LOAD_IsOpen_RacingOrBattle();
		if (uVar5 != 0)
		{
			RB_Player_ToggleInvisible();
			RB_Player_ToggleFlicker();
			RB_Burst_ProcessBucket(sdata->gGT->threadBuckets[7].thread);
			RB_Blowup_ProcessBucket(sdata->gGT->threadBuckets[8].thread);
			RB_Spider_DrawWebs(sdata->gGT->threadBuckets[10].thread, gGT->tileView);
			RB_Follower_ProcessBucket(sdata->gGT->threadBuckets[0xb].thread);
			RB_StartText_ProcessBucket(sdata->gGT->threadBuckets[0xc].thread);
		}
		uVar5 = LOAD_IsOpen_AdvHub();
		if ((uVar5 != 0) && ((sdata->gGT->gameMode1 & 0x100000) != 0))
		{
			AH_WarpPad_AllWarppadNum();
		}
		iVar20 = 0;
		Turbo_ProcessBucket(sdata->gGT->threadBuckets[9].thread);
		if (gGT->numPlyrCurrGame != 0)
		{
			iVar17 = 0x168;
			do
			{
				psVar15 = (struct TileView *)((int)gGT->db[0].drawEnv.ofs + iVar17 + -0x20);
				DrawSkidMarks_Main(sdata->gGT->threadBuckets[0].thread, psVar15);
				iVar17 = iVar17 + 0x110;
				DrawSkidMarks_Main(sdata->gGT->threadBuckets[1].thread, psVar15);
				iVar20 = iVar20 + 1;
			} while (iVar20 < (int)(u_int)(u_char)gGT->numPlyrCurrGame);
		}
	}
	if (((sdata->gGT->gameMode1 & 0x2000) != 0) && (uVar5 = LOAD_IsOpen_MainMenu(),  uVar5 != 0))
	{
		MM_Title_SetTrophyDPP();
	}
	if ((gGT->renderFlags & 0x20) != 0)
	{
		RenderBucket_Execute(sdata->gGT->ptrRenderBucketInstance, &sdata->gGT->backBuffer->primMem);
	}
	if ((gGT->renderFlags & 0x80) != 0)
	{
		psVar14 = sdata->gGT->threadBuckets[0].thread;
		if (psVar14 != (struct Thread *)0)
		{
			DrawTires_Solid(psVar14, &sdata->gGT->backBuffer->primMem, gGT->numPlyrCurrGame);
			DrawTires_Reflection(sdata->gGT->threadBuckets[0].thread, &sdata->gGT->backBuffer->primMem, gGT->numPlyrCurrGame);
		}
		if (sdata->gGT->numBotsNextGame != 0)
		{
			DrawTires_Solid(sdata->gGT->threadBuckets[1].thread, &sdata->gGT->backBuffer->primMem, gGT->numPlyrCurrGame);
			DrawTires_Reflection(sdata->gGT->threadBuckets[1].thread, &sdata->gGT->backBuffer->primMem, gGT->numPlyrCurrGame);
		}
		psVar14 = sdata->gGT->threadBuckets[2].thread;
		if (psVar14 != (struct Thread *)0)
		{
			DrawTires_Solid(psVar14, &sdata->gGT->backBuffer->primMem, gGT->numPlyrCurrGame);
			DrawTires_Reflection(sdata->gGT->threadBuckets[2].thread, &sdata->gGT->backBuffer->primMem, gGT->numPlyrCurrGame);
		}
	}
	if ((gGT->renderFlags & 0x400) != 0)
	{
		DrawShadows_Main();
	}
	if ((gGT->renderFlags & 0x800) != 0)
	{
		DrawHeat_Main(sdata->gGT->particleList_heatWarp, gGT->tileView, &sdata->gGT->backBuffer->primMem, gGT->numPlyrCurrGame, sdata->gGT->swapchainIndex * 0x128);
	}
	Camera110_FadeAllWindows();
	if (((gGT->renderFlags & 1) != 0) && (playstationVar23 != (struct mesh_info *)0))
	{
		bVar2 = sdata->gGT->numPlyrCurrGame;
		if (bVar2 == 2)
		{
			CTR_ClearRenderLists_1P2P(gGT, 2);
			if ((level->configFlags & 4) == 0)
			{
				AnimateWater2P(gGT->timer, level->count_water, level->ptr_water, level->ptr_tex_waterEnvMap, gGT->visMem1->visOVertList[0], gGT->visMem1->visOVertList[1]);
			}
			iVar17 = 0;
			Visdata->CopyJMPsToScratchpad();
			gGT->numVisDataLinks = 0;
			iVar19 = 0x1808;
			iVar20 = 0x168;
			do
			{
				iVar21 = iVar20 + -0x20;
				ppiVar13 = gGT->visMem1->visLeafList + iVar17;
				iVar11 = iVar19 + -0x20;
				iVar19 = iVar19 + 0x30;
				iVar20 = iVar20 + 0x110;
				iVar21 = CreateRenderLists_1P2P((struct VisData *)(&playstationVar23->ptrVertexArray)[2], *ppiVar13, (struct TileView *)((int)gGT->db[0].drawEnv.ofs + iVar21), (int)gGT->db[0].drawEnv.ofs + iVar11, ppiVar13[0x20], gGT->numPlyrCurrGame);
				iVar17 = iVar17 + 1;
				gGT->numVisDataLinks = gGT->numVisDataLinks + iVar21;
			} while (iVar17 < 2);
			iVar20 = 0;
			iVar17 = 0x168;
			DrawLevelPrims_EntryFunc(gGT->LevRenderLists, gGT->tileView, (struct VisData *)playstationVar23, &gGT->backBuffer->primMem, gGT->visMem1->visFaceList[0], gGT->visMem1->visFaceList[1], level->ptr_tex_waterEnvMap);
			psVar18 = gGT;
			do
			{
				iVar19 = iVar17 + -0x20;
				psVar15 = psVar18->tileView;
				psVar18 = (struct GameTracker *)(psVar18->db[1].drawEnv.dr_env.code + 0xd);
				iVar17 = iVar17 + 0x110;
				iVar20 = iVar20 + 1;
				CAM_SkyboxGlow(&level->glowGradient[0].pointFrom, (struct TileView *)((int)gGT->db[0].drawEnv.ofs + iVar19), &gGT->backBuffer->primMem, (u_long *)((int)psVar15->ptrOT + 0xffc));
			} while (iVar20 < 2);
		}
		else if (bVar2 < 3)
		{
			if (bVar2 == 1)
			{
				CTR_ClearRenderLists_1P2P(gGT, 1);
				if (((level->configFlags & 4) == 0) && (level != (struct Level *)0))
				{
					AnimateWater1P(gGT->timer, level->count_water, level->ptr_water, level->ptr_tex_waterEnvMap, gGT->visMem1->visOVertList[0]);
				}
				else
				{
					AnimateQuad(gGT->timer << 7, level->numSCVert, level->ptrSCVert, (int *)gGT->visMem1->visSCVertList[0]);
				}
				iVar20 = sdata->gGT->tileView[0].distanceToScreen_PREV;
				if ((sdata->gGT->levelID == 0x28) || (((sdata->gGT->gameMode1 & 0x20000000) != 0 && (sdata->gGT->levelID != 0x25))))
				{
					// relationship between near-clip and far-clip,
					// for each RenderList LOD set in the level
					*(int*)0x1f800014 = 0x1e00;
					*(int*)0x1f800018 = 0x640;
					*(int*)0x1f80001c = 0x640;
					*(int*)0x1f800020 = 0x500;
					*(int*)0x1f800024 = 0x280;
					*(int*)0x1f800028 = 0x140;
					*(int*)0x1f80002c = 0x640+0x140;
				}
				else
				{
					iVar17 = iVar20 * 0x2080;
					if (iVar17 < 0)
					{
						iVar17 = iVar17 + 0xff;
					}
					*(int*)0x1f800024 = iVar20 * 7;
					iVar19 = iVar20 * 0x380;
					*(int*)0x1f800014 = iVar17 >> 8;
					*(int*)0x1f800020 = iVar20 * 0xc;
					*(int*)0x1f800018 = iVar20 * 0x1a;
					*(int*)0x1f80001c = iVar20 * 0x18;
					if (iVar19 < 0)
					{
						iVar19 = iVar19 + 0xff;
					}
					*(int*)0x1f800028 = iVar19 >> 8;
					*(int*)0x1f80002c = *(int*)0x1f800018 + 0x140;
				}
				Visdata->CopyJMPsToScratchpad();
				psVar15 = gGT->tileView;
				iVar20 = CreateRenderLists_1P2P((struct VisData *)(&playstationVar23->ptrVertexArray)[2], gGT->visMem1->visLeafList[0], psVar15, (u_int)gGT->LevRenderLists, gGT->visMem1->bspList[0], gGT->numPlyrCurrGame);
				gGT->numVisDataLinks = iVar20;
				DrawLevelPrims_EntryFunc(gGT->LevRenderLists, psVar15, (struct VisData *)level->ptr_mesh_info, &gGT->backBuffer->primMem, gGT->visMem1->visFaceList[0], (int *)level->ptr_tex_waterEnvMap, in_stack_ffffffb0);
				DrawSky_Full(level->ptr_skybox, psVar15, &gGT->backBuffer->primMem);
				if ((level->configFlags & 1) != 0)
				{
					CAM_SkyboxGlow(&level->glowGradient[0].pointFrom, psVar15, &gGT->backBuffer->primMem, (u_long *)((int)gGT->tileView[0].ptrOT + 0xffc));
				}
			}
		}
		else if (bVar2 == 3)
		{
			CTR_ClearRenderLists_3P4P(gGT, 3);
			if ((level->configFlags & 4) == 0)
			{
				AnimateWater3P(gGT->timer, level->count_water, level->ptr_water, level->ptr_tex_waterEnvMap, gGT->visMem1->visOVertList[0], gGT->visMem1->visOVertList[1], gGT->visMem1->visOVertList[2]);
			}
			iVar17 = 0;
			Visdata->CopyJMPsToScratchpad();
			gGT->numVisDataLinks = 0;
			iVar19 = 0x1808;
			iVar20 = 0x168;
			do
			{
				iVar21 = iVar20 + -0x20;
				iVar11 = iVar19 + -0x20;
				ppiVar13 = gGT->visMem1->visLeafList + iVar17;
				iVar19 = iVar19 + 0x30;
				iVar20 = iVar20 + 0x110;
				iVar21 = CreateRenderLists_3P4P((struct VisData *)(&playstationVar23->ptrVertexArray)[2], *ppiVar13, (struct TileView *)((int)gGT->db[0].drawEnv.ofs + iVar21), (int)gGT->db[0].drawEnv.ofs + iVar11, ppiVar13[0x20]);
				iVar17 = iVar17 + 1;
				gGT->numVisDataLinks = gGT->numVisDataLinks + iVar21;
			} while (iVar17 < 3);
			iVar20 = 0;
			iVar17 = 0x168;
			DrawLevelPrims_EntryFunc(gGT->LevRenderLists, gGT->tileView, (struct VisData *)playstationVar23, &gGT->backBuffer->primMem, gGT->visMem1->visFaceList[0], gGT->visMem1->visFaceList[1], gGT->visMem1->visFaceList[2]);
			psVar18 = gGT;
			do
			{
				iVar19 = iVar17 + -0x20;
				psVar15 = psVar18->tileView;
				psVar18 = (struct GameTracker *)(psVar18->db[1].drawEnv.dr_env.code + 0xd);
				iVar17 = iVar17 + 0x110;
				iVar20 = iVar20 + 1;
				CAM_SkyboxGlow(&level->glowGradient[0].pointFrom, (struct TileView *)((int)gGT->db[0].drawEnv.ofs + iVar19), &gGT->backBuffer->primMem, (u_long *)((int)psVar15->ptrOT + 0xffc));
			} while (iVar20 < 3);
		}
		else if (bVar2 == 4)
		{
			CTR_ClearRenderLists_3P4P(gGT, 4);
			if ((level->configFlags & 4) == 0)
			{
				AnimateWater4P(gGT->timer, level->count_water, level->ptr_water, level->ptr_tex_waterEnvMap, gGT->visMem1->visOVertList[0], gGT->visMem1->visOVertList[1], gGT->visMem1->visOVertList[2], gGT->visMem1->visOVertList[3]);
			}
			iVar17 = 0;
			Visdata->CopyJMPsToScratchpad();
			gGT->numVisDataLinks = 0;
			iVar19 = 0x1808;
			iVar20 = 0x168;
			do
			{
				iVar21 = iVar20 + -0x20;
				iVar11 = iVar19 + -0x20;
				ppiVar13 = gGT->visMem1->visLeafList + iVar17;
				iVar19 = iVar19 + 0x30;
				iVar20 = iVar20 + 0x110;
				iVar21 = CreateRenderLists_3P4P((struct VisData *)(&playstationVar23->ptrVertexArray)[2], *ppiVar13, (struct TileView *)((int)gGT->db[0].drawEnv.ofs + iVar21), (int)gGT->db[0].drawEnv.ofs + iVar11, ppiVar13[0x20]);
				iVar17 = iVar17 + 1;
				gGT->numVisDataLinks = gGT->numVisDataLinks + iVar21;
			} while (iVar17 < 4);
			iVar20 = 0;
			iVar17 = 0x168;
			DrawLevelPrims_EntryFunc(gGT->LevRenderLists, gGT->tileView, (struct VisData *)playstationVar23, &gGT->backBuffer->primMem, gGT->visMem1->visFaceList[0], gGT->visMem1->visFaceList[1], gGT->visMem1->visFaceList[2]);
			psVar18 = gGT;
			do
			{
				iVar19 = iVar17 + -0x20;
				psVar15 = psVar18->tileView;
				psVar18 = (struct GameTracker *)(psVar18->db[1].drawEnv.dr_env.code + 0xd);
				iVar17 = iVar17 + 0x110;
				iVar20 = iVar20 + 1;
				CAM_SkyboxGlow(&level->glowGradient[0].pointFrom, (struct TileView *)((int)gGT->db[0].drawEnv.ofs + iVar19), &gGT->backBuffer->primMem, (u_long *)((int)psVar15->ptrOT + 0xffc));
			} while (iVar20 < 4);
		}
		iVar20 = 0;
		if (gGT->numPlyrCurrGame != 0)
		{
			psVar15 = gGT->tileView;
			psVar18 = gGT;
			iVar17 = 0x168;
			do
			{
				psVar3 = psVar18->tileView;
				psVar18 = (struct GameTracker *)(psVar18->db[1].drawEnv.dr_env.code + 0xd);
				Camera110_SetDrawEnv_Normal((u_long *)((int)psVar3->ptrOT + 0xffc), psVar15, gGT->backBuffer, (short *)0, 0);
				iVar20 = iVar20 + 1;
				psVar15 = (struct TileView *)((int)gGT->tileView[0].pos + iVar17 + -0x58);
				iVar17 = iVar17 + 0x110;
			} while (iVar20 < (int)(u_int)(u_char)gGT->numPlyrCurrGame);
		}
		if (((sdata->gGT->hudFlags & 1) != 0) && (1 < (u_char)sdata->gGT->numPlyrCurrGame))
		{
			DrawHUD_Wumpa3D_2P3P4P(gGT);
		}
		if (((gGT->renderFlags & 0x100) != 0) && (1 < (u_char)sdata->gGT->numPlyrCurrGame))
		{
			DecalMP_03(gGT);
		}
		if (((gGT->gameMode1 & 0x20102000) == 0) && (sdata->Loading_stage != 0xfffffffc))
		{
			DotLights_AudioAndVideo(gGT);
		}
		if ((sdata->gGT->renderFlags & 0x8000) != 0)
		{
			if ((((sdata->gGT->gameMode1 & 0x20) != 0) && (2 < (u_char)sdata->gGT->numPlyrCurrGame)) && (iVar20 = 0,  sdata->gGT->numPlyrCurrGame != 0))
			{
				iVar17 = 0;
				do
				{
					local_30.x = *(short *)((int)sdata->gGT->tileView[0].matrix_ViewProj.m[-2] + iVar17);
					local_30.y = *(short *)((int)sdata->gGT->tileView[0].matrix_ViewProj.m[-2] + iVar17 + 2);
					local_30.w = *(short *)((int)sdata->gGT->tileView[0].matrix_ViewProj.m[-2] + iVar17 + 4);
					local_30.h = *(short *)((int)sdata->gGT->tileView[0].matrix_ViewProj.m[-1] + iVar17);
					DrawBoxOutline_LowLevel(&local_30, 4, 2, data_ptrColors[(sdata->gGT->drivers[iVar20]->BattleHUD).teamID + 0x18], 0, (u_long *)((int)(sdata->gGT->tileView_UI).ptrOT + 0xc));
					iVar20 = iVar20 + 1;
					iVar17 = iVar17 + 0x110;
				} while (iVar20 < (int)(u_int)(u_char)sdata->gGT->numPlyrCurrGame);
			}
			if (1 < (u_char)sdata->gGT->numPlyrCurrGame)
			{
				pvVar9 = (sdata->gGT->backBuffer->primMem).curr;
				*(u_char *)((int)pvVar9 + 3) = 5;
				*(u_char *)((int)pvVar9 + 7) = 0x28;
				*(u_char *)((int)pvVar9 + 6) = 0;
				*(u_char *)((int)pvVar9 + 5) = 0;
				*(u_char *)((int)pvVar9 + 4) = 0;
				psVar18 = sdata->gGT;
				*(u_short *)((int)pvVar9 + 10) = 0x6a;
				*(u_short *)((int)pvVar9 + 0xe) = 0x6a;
				*(u_short *)((int)pvVar9 + 8) = 0;
				*(u_short *)((int)pvVar9 + 0xc) = 0x200;
				*(u_short *)((int)pvVar9 + 0x10) = 0;
				*(u_short *)((int)pvVar9 + 0x12) = 0x6e;
				*(u_short *)((int)pvVar9 + 0x14) = 0x200;
				*(u_short *)((int)pvVar9 + 0x16) = 0x6e;
				AddPrim((void *)((int)(psVar18->tileView_UI).ptrOT + 0xc), pvVar9);
				(sdata->gGT->backBuffer->primMem).curr = (void *)((int)(sdata->gGT->backBuffer->primMem).curr + 0x18);
			}
			if (2 < (u_char)sdata->gGT->numPlyrCurrGame)
			{
				pvVar9 = (sdata->gGT->backBuffer->primMem).curr;
				*(u_char *)((int)pvVar9 + 3) = 5;
				*(u_char *)((int)pvVar9 + 7) = 0x28;
				*(u_char *)((int)pvVar9 + 6) = 0;
				*(u_char *)((int)pvVar9 + 5) = 0;
				*(u_char *)((int)pvVar9 + 4) = 0;
				psVar18 = sdata->gGT;
				*(u_short *)((int)pvVar9 + 8) = 0xfd;
				*(u_short *)((int)pvVar9 + 0x10) = 0xfd;
				*(u_short *)((int)pvVar9 + 10) = 0;
				*(u_short *)((int)pvVar9 + 0xc) = 0x103;
				*(u_short *)((int)pvVar9 + 0xe) = 0;
				*(u_short *)((int)pvVar9 + 0x12) = 0xd8;
				*(u_short *)((int)pvVar9 + 0x14) = 0x103;
				*(u_short *)((int)pvVar9 + 0x16) = 0xd8;
				AddPrim((void *)((int)(psVar18->tileView_UI).ptrOT + 0xc), pvVar9);
				(sdata->gGT->backBuffer->primMem).curr = (void *)((int)(sdata->gGT->backBuffer->primMem).curr + 0x18);
			}
			if (sdata->gGT->numPlyrCurrGame == '\x03')
			{
				pvVar9 = (sdata->gGT->backBuffer->primMem).curr;
				*(u_char *)((int)pvVar9 + 3) = 5;
				*(u_char *)((int)pvVar9 + 7) = 0x28;
				*(u_char *)((int)pvVar9 + 6) = 0;
				*(u_char *)((int)pvVar9 + 5) = 0;
				*(u_char *)((int)pvVar9 + 4) = 0;
				*(u_short *)((int)pvVar9 + 8) = 0x100;
				*(u_short *)((int)pvVar9 + 0x10) = 0x100;
				psVar18 = sdata->gGT;
				*(u_short *)((int)pvVar9 + 10) = 0x6c;
				*(u_short *)((int)pvVar9 + 0xe) = 0x6c;
				*(u_short *)((int)pvVar9 + 0xc) = 0x200;
				*(u_short *)((int)pvVar9 + 0x12) = 0xd8;
				*(u_short *)((int)pvVar9 + 0x14) = 0x200;
				*(u_short *)((int)pvVar9 + 0x16) = 0xd8;
				AddPrim((void *)((int)(psVar18->tileView_UI).ptrOT + 0xc), pvVar9);
				(sdata->gGT->backBuffer->primMem).curr = (void *)((int)(sdata->gGT->backBuffer->primMem).curr + 0x18);
			}
		}
		if (sdata->Loading_stage == 0xffffffff)
		{
			if ((sdata->gGT->gameMode1 & (PAUSE_1 | PAUSE_2 | PAUSE_3 | PAUSE_4)) == 0)
			{
				RivalWeapons_Update();
			}
			StartLine_Update();
		}
	}
	if ((sdata->gGT->gameMode1 & 0x302000) != 0)
	{
		unk80047d64();
	}
	if (((sdata->gGT->renderFlags & 0x2000) != 0) && ((gGT->level1->clearColor[0].enable != 0 || (gGT->level1->clearColor[1].enable != 0))))
	{
		CAM_ClearScreen(gGT);
	}
	if ((sdata->gGT->renderFlags & 0x1000) != 0)
	{
		CheckeredFlag_DrawSelf();
	}
	Camera110_SetDrawEnv_Normal((u_long *)((int)(gGT->tileView_UI).ptrOT + 0x10), &gGT->tileView_UI, gGT->backBuffer, (short *)0, 0);
	iVar20 = RCNT_GetTime_Total();
	psVar18 = sdata->gGT;
	gGT->countTotalTime = iVar20;
	if ((psVar18->renderFlags & 0x1000) != 0)
	{
		VSync(0);
	}
	cVar4 = sdata->gGT->bool_DrawOTag_InProgress;
	while(1)
	{
		if ((cVar4 == 0) && (sdata->vsyncTillFlip < 1)) goto LAB_800378d0;
		if (6 < (u_int)sdata->gGT->vSync_between_drawSync) break;
		cVar4 = sdata->gGT->bool_DrawOTag_InProgress;
	}
	BreakDraw();
LAB_800378d0:
	if (sdata->boolPlayVideoSTR == 1)
	{
		MM_Video_CheckIfFinished(1);
		MoveImage(&sdata->videoSTR_src_vramRect, sdata->videoSTR_dst_vramX, sdata->videoSTR_dst_vramY);
		DrawSync(0);
	}
	iVar20 = RCNT_GetTime_Elapsed(gGT->countTotalTime, (void *)0);
	gGT->countTotalTime = iVar20;
	if (gGT->frontBuffer != (struct DB *)0)
	{
		sdata->vsyncTillFlip = 2;
		uVar6 = DAT_8008cff0 & 1;
		sdata->gGT->unk1cc4[5] = sdata->gGT->unk1cc4[0];
		if (uVar6 == 0)
		{
			env = &gGT->frontBuffer->dispEnv;
		}
		else
		{
			env = &sdata->blank_debug_DispEnv;
		}
		PutDispEnv(env);
		PutDrawEnv(&gGT->frontBuffer->drawEnv);
		gGT->frontBuffer = (struct DB *)0;
	}
	psVar18 = sdata->gGT;
	gGT->frontBuffer = (struct DB *)((int)gGT - (gGT->swapchainIndex * 0xa4 + -0xbc));
	psVar18->bool_DrawOTag_InProgress = 1;
	DrawOTag((u_long *)((int)gGT->tileView[0].ptrOT + 0xffc));
	gGT->frameTimer_notPaused = gGT->frameTimer_VsyncCallback;
	return;
}