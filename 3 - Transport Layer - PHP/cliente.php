<?php
require_once("pacote.php");

	$host = "127.0.0.1";
	$port = "11001";
	$hostSend = "127.0.0.1";
	$portSend = 12001;

	//socket que recebe da camada de aplicação
	$socketRecv = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
	socket_bind($socketRecv, $host, $port);
	socket_listen($socketRecv, SOMAXCONN);

	echo "Esperando camada de aplicação\n";

	$connection;
	$content;
	$lim_bytes = 1000000000; //1 GB
	$bypass = false;

	//conexão com a aplicaçãonetwork
	while(($connection = socket_accept($socketRecv)) != FALSE) {
		echo "Aplicação enviando\n";
		$content = socket_read($connection, $lim_bytes, PHP_BINARY_READ);

		//socket pra enviar pra camada de rede
		$socketSend = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die(socket_strerror(socket_last_error())."\n");
		echo "Socket criado\n";
		socket_connect($socketSend, $hostSend, $portSend) or die(socket_strerror(socket_last_error($socketSend))."\n");
		echo "Conectado a $hostSend.\n";

		$pacote = new Pacote;
		$pacote->set($portSend, $port, "", $content);
		var_dump($pacote);

		if(!$bypass) {
			socket_write($socketSend, $pacote->toString(), strlen($pacote->toString()));
		} else {
			$syn = new Pacote;
			$syn->set($portSend, $port, "SYN", "oi");
			echo "syn\n";
			var_dump($syn);
			socket_write($socketSend, $syn->toString(), strlen($syn->toString()));

			$resposta = socket_read($socketSend, $lim_bytes, PHP_BINARY_READ);
			$ackServer = new Pacote;
			$ackServer->convert($resposta);

			$ack = new Pacote;
			$ack->set($portSend, $port, "ACK", "oi");
			socket_write($socketSend, $ack->toString(), strlen($ack->toString()));
			//TODO setar o ACK e num de sequência e fazer controle de fluxo
			socket_write($socketSend, $pacote->toString(), strlen($pacote->toString()));
		}

		$resposta = socket_read($socketSend, $lim_bytes, PHP_BINARY_READ);

		$packet2 = new Pacote;
		$packet2->convert($resposta);
		$unpacked = $packet2->dado;
		if(socket_write($connection, $unpacked, strlen($unpacked)) === false) echo "Erro de envio para aplicação";
		//socket_close($socketSend);

		//socket_close($connection);
	}

	//socket_close($socketRecv);
?>
