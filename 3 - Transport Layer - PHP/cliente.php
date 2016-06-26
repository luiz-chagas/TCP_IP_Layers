<?php
require_once("pacote.php");

	$host = "127.0.0.1";
	$port = "11001";
	$hostSend = "127.0.0.1";
	$portSend = 11000;

	$socketRecv = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
	$lim_bytes = 1000000000; //1 GB

	socket_bind($socketRecv, $host, $port);
	socket_listen($socketRecv, SOMAXCONN);

	echo "Esperando camada de aplicação\n";

	$connection;
	$content;
	$bypass = false;
	
	while(($connection = socket_accept($socketRecv)) != FALSE) {
		echo "Aplicação enviando\n";
		$content = socket_read($connection, $lim_bytes, PHP_BINARY_READ);

		$socketSend = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die(socket_strerror(socket_last_error())."\n");
		echo "Socket criado\n";

		socket_connect($socketSend, $host, $portSend) or die("Could not connect to socket");
		
		$result = socket_listen($socketSend) or die(socket_strerror(socket_last_error($socketSend))."\n");
		echo "Conectado a $hostSend.\n";

		$ouvindo = socket_accept($socketSend) or die(socket_strerror(socket_last_error($socketSend))."\n");

		$pacote = new Pacote;
		$pacote->set($portSend, $port, "", $content);
		var_dump($pacote);
		
		if($bypass) {
			socket_write($ouvindo, $pacote->toString(), strlen($pacote->toString()));
		} else {
			$syn = new Pacote;
			$syn->set($portSend, $port, "SYN", "oi");
			echo "syn\n";
			var_dump($syn);
			socket_write($ouvindo, $syn->toString(), strlen($syn->toString()));

			$resposta = socket_read($ouvindo, 1000000000, PHP_BINARY_READ);
			$ackServer = new Pacote;
			$ackServer->convert($resposta);

			$ack = new Pacote;
			$ack->set($portSend, $port, "ACK", "oi");
			socket_write($ouvindo, $ack->toString(), strlen($ack->toString()));
			//TODO setar o ACK e num de sequência e fazer controle de fluxo
			socket_write($ouvindo, $pacote->toString(), strlen($pacote->toString()));
		}
		
		echo "Transmissão finalizada\n";
		socket_close($socketSend);

		socket_close($connection);
	}

	socket_close($socketRecv);
?>
