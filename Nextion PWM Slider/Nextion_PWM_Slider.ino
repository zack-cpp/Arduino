class myData{
  public:
  String dataReceived;
  int value;
  char charReceived;
  char ascii[50] = {
    '(',')','*','+',',','-','.','/',
    '0','1','2','3','4','5','6','7',
    '8','9',':',';','<','=','>','?',
    '@','A','B','C','D','E','F','G',
    'H','I','K','L','M','N','O','P',
    'Q','R','S','T','U','V','W','X',
    'Y','Z'
  };
}data;
int AsConv();

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  while(!Serial.available()){
    delay(100);
    data.dataReceived = "";
    data.charReceived = "";
  }
  while(Serial.available()){
    data.charReceived = (char)Serial.read();
    data.dataReceived += data.charReceived;
  }
  data.value = AsConv(data.charReceived);
  data.value = map(data.value, 0, 49, 0, 100);
  Serial.print("Value: ");
  Serial.println(data.value);
}
int AsConv(char dataa){
  for(int i = 0; i < 50; i++){
    if(dataa == data.ascii[i]){
      return i;
      break;
    }
  }
}
