# Spectrum

* Para funcionar:

	* colocar na pasta projetos da libUnicornio
	
	* compilar para criar uma pasta bin e colar arquivos da pasta bin de algum projeto da libUnicornio nesta pasta criada
	
	* adicionar libfftw3-3.dll nesta pasta
	
	* criar pastas Musics e Files dentro da pasta bin
	
	* trocar ./ por ./bin/ em:
	
		* main.cpp: linhas 29, 30 e 46
		
		* wav.cpp: linhas 61 e 80
	
	* mudar a música na linha const char* path = ... no main.cpp
	
	* por algum motivo, janela de debug não aparece depois de passar pro Github

* Referências:
	
	* http://soundfile.sapp.org/doc/WaveFormat/
	
	* https://stackoverflow.com/questions/36949957/loading-a-wav-file-for-openal
	
	* https://github.com/dilawar/wav/blob/master/WAVReader.h
	
	* https://www.youtube.com/watch?v=CMyG4hsKCJo
	
	* https://www.youtube.com/playlist?list=PLT5_DQAJJLh-ogHjHcLtFYMQy7SkZ7-3i
