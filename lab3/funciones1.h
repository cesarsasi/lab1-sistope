float calculoMediaReal(float matriz[3][5], int largo){
	float sumaReal=0.0;
	float largoF= (float)largo;
	float mediaR;
	for(int i=0;i<largo;i++){
		sumaReal=sumaReal+matriz[i][2];
	}
	mediaR = sumaReal/largoF;
	return mediaR;
}
float calculoMediaImaginaria(float matriz[3][5], int largo){
	float sumaImaginaria =0.0;
	float largoF=(float)largo;
	float mediaI;
	for(int i=0;i<largo;i++){
		sumaImaginaria=sumaImaginaria+matriz[i][3];
	}
	mediaI= sumaImaginaria/largoF;
	return mediaI;
}
