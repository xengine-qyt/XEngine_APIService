﻿#pragma once
void CALLBACK HTTPTask_TaskPost_CBVideo(uint8_t* punStringY, int nYLen, uint8_t* punStringU, int nULen, uint8_t* punStringV, int nVLen, XPVOID lParam);
void CALLBACK HTTPTask_TaskPost_CBAudio(uint8_t* punStringAudio, int nVLen, XPVOID lParam);

bool HTTPTask_TaskPost_BackService(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nType);