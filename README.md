# Spectrum

* Para funcionar:

	* compilar para criar uma pasta bin e colar arquivos da pasta bin de algum projeto da libUnicornio nesta pasta criada
	
	* adicionar libfftw3-3.dll nesta pasta
	
	* criar pasta include contendo fftw3.h e pasta lib contendo libfftw3-3.lib
	
	* criar pastas Musics e Files dentro da pasta bin
	
	* trocar ./Musics por ./bin/Musics nas linhas 50-63 do main.cpp
	
	* trocar ./Files por ./bin/Files nas linhas 96, 113, 122, 141 e 150 do AudioManager.h e 62 do Utils.cpp

* Referências:

	* https://www.youtube.com/watch?v=CMyG4hsKCJo
	
	* https://www.youtube.com/playlist?list=PLT5_DQAJJLh-ogHjHcLtFYMQy7SkZ7-3i
