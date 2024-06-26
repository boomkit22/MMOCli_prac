// Fill out your copyright notice in the Description page of Project Settings.


#include "DataStructure/TlsObjectPool.h"

TlsLog _objectPoolMonitor[MAX_TLS_LOG_INDEX] = {};
int releaseStackSize;