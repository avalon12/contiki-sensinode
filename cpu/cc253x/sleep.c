/* This method must be 8 byte aligned, we do this by setting it in a 
seperate file and telling the compiler to map the function to 
an absolute address this is 8 byte aligned */
#pragma codeseg 0x7FF8

void startSleep(){
  PCON = 0x01;
}
