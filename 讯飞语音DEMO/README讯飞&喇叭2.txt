器件：机器人

目的：了解机器人喇叭发声实验及讯飞语音的使用
          

硬件资源：[1]烧写工具，串口工具
         
软件资源：[1]STM32 ST-LINK Utility、[2]XCOM V2.0.exe 、[3]讯飞语音

现象：机器人语音播放出字符串：“hello owenlzy”
        void  App_xfTTSTaskCreate (void);调用任务：uctsk_xfTTS，并启动任务。

 

	void ReadText(char* str, int encoding)； 依次发送待合成的文本数据



	static void uctsk_xfTTS (void) ；完成字符串"hello owenlzy"的输入，并调用 ReadText(str, 0x00);
	发声完成。

