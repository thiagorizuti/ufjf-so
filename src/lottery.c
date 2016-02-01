/*
*  lottery.c - Implementacao do algoritmo Lottery Scheduling e sua API
*
*  Autores: Raphael, Tales, Thiago
*  Projeto: Trabalho Pratico I - Sistemas Operacionais
*  Organizacao: Universidade Federal de Juiz de Fora
*  Departamento: Dep. Ciencia da Computacao
*
*
*/

#include "lottery.h"
#include <stdio.h>
#include <string.h>

//Nome unico do algoritmo. Deve ter 4 caracteres.
const char lottName[]="LOTT";

int slot;

int ticketsTotal;

//=====Funcoes Auxiliares=====

int ticketsCount(Process *plist){
	int count = 0;
	Process* process = plist;

	while(process != NULL){
		if(processGetSchedSlot(process) ==  slot && (processGetStatus(process) == PROC_READY || processGetStatus(process) == PROC_RUNNING)){
			count += ((LotterySchedParams*) processGetSchedParams(process))->num_tickets;
		}
		process = processGetNext(process);
	}

	return count;
}




//=====Funcoes da API=====

//Funcao chamada pela inicializacao do S.O. para a incializacao do escalonador
//conforme o algoritmo Lottery Scheduling
//Deve envolver a inicializacao de possiveis parametros gerais
//Deve envolver o registro do algoritmo junto ao escalonador
void lottInitSchedInfo() {
	SchedInfo *lottSI=  malloc (sizeof(SchedInfo));
	strcpy(lottSI->name,lottName);
	lottSI->initParamsFn = lottInitSchedParams;
	lottSI->scheduleFn = lottSchedule;
	lottSI->releaseParamsFn = lottReleaseParams;
	slot = schedRegisterScheduler(lottSI);
}

//Inicializa os parametros de escalonamento de um processo p, chamada 
//normalmente quando o processo e' associado ao slot de Lottery
void lottInitSchedParams(Process *p, void *params) {
	processSetSchedParams(p,params);
	processSetSchedSlot(p,slot);
}

//Retorna o proximo processo a obter a CPU, conforme o algortimo Lottery 
Process* lottSchedule(Process *plist) {
	Process* process = plist;
	int randomTicket;

	ticketsTotal = ticketsCount(plist);

	randomTicket = rand()%ticketsTotal;

	while(process != NULL){
		if(processGetSchedSlot(process) ==  slot && (processGetStatus(process) == PROC_READY || processGetStatus(process) == PROC_RUNNING)){
			randomTicket -=((LotterySchedParams*) processGetSchedParams(process))->num_tickets;
			if(randomTicket <= 0){
				return process;
			} 
		}
		process = processGetNext(process);
	}

	return NULL;
}

//Libera os parametros de escalonamesnto de um processo p, chamada 
//normalmente quando o processo e' desassociado do slot de Lottery
//Retorna o numero do slot ao qual o nprocesso estava associado
int lottReleaseParams(Process *p) {
	LotterySchedParams* lottParams = (LotterySchedParams*) processGetSchedParams(p);
	ticketsTotal -= lottParams->num_tickets;
	free(lottParams);
	return processGetSchedSlot(p);
}

//Transfere certo numero de tickets do processo src para o processo dst.
//Retorna o numero de tickets efetivamente transfeirdos (pode ser menos)
int lottTransferTickets(Process *src, Process *dst, int tickets) {
	LotterySchedParams* srcParams = processGetSchedParams(src);
	LotterySchedParams* dstParams = processGetSchedParams(dst);

	if(srcParams->num_tickets > 0){
		if(srcParams->num_tickets - tickets < 0){
			tickets += srcParams->num_tickets - tickets - 1;
		}
			srcParams->num_tickets -= tickets;
			dstParams->num_tickets += tickets;
			return tickets;
	}
	return 0;
}
