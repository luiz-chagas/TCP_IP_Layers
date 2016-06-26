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
	while(($connection = socket_accept($socketRecv)) != FALSE) {
		echo "Aplicação enviando\n";
		$content = socket_read($connection, $lim_bytes, PHP_BINARY_READ);

		//socket para enviar criado com UDP para a funcionalidade ser implementada pela minha função
		$socketSend = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
		if ($socketSend === false) {
			echo socket_strerror(socket_last_error()) . "\n";
		} else {
			echo "Socket criado\n";
		}

		$result = socket_connect($socketSend, $hostSend, $portSend);
		if ($result === false) {
			echo socket_strerror(socket_last_error($socketSend)) . "\n";
		} else {
			echo "Conectado a $hostSend.\n";
		}

		$pacote = new Pacote;
		$pacote->set($portSend, $port, "", $content);
		var_dump($pacote);
		//usar essa pra UDP
		bypass($socketSend, $pacote->toString());
		//usar essa pra TCP
		//bypass($socketSend, $pacote->toString());

		echo "Transmissão finalizada\n";
		socket_close($socketSend);

		socket_close($connection);
	}

	socket_close($socketRecv);

	function bypass($sock, $conteudo) {
		socket_write($sock, $conteudo, strlen($conteudo));
	}

	function handshake($socket, $packet) {
		$syn = new Pacote;
		$syn->set($portSend, $port, "SYN", "oi");
		socket_write($socket, $syn->toString(), strlen($syn->toString()));

		$ouvindo = socket_accept($socket);
		$resposta = socket_read($ouvindo, $lim_bytes, PHP_BINARY_READ);
		$ackServer = new Pacote;
		$ackServer->convert($resposta);

		$ack = new Pacote;
		$ack->set($portSend, $port, "ACK", "oi");
		socket_write($socket, $ack, strlen($ack));
		//TODO setar o ACK e num de sequência e fazer controle de fluxo
		socket_write($sock, $packet, strlen($packet));
	}
?>
