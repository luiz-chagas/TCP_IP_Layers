<?php
require_once("pacote.php");

	$host = "127.0.0.1";
	$port = "11000";
	$hostSend = "127.0.0.1";
	$portSend = 10000;

	$socketRecv = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
	socket_bind($socketRecv, $host, $port);
	socket_listen($socketRecv, SOMAXCONN);
	echo "Esperando a camada de rede\n";

	$connection;
	$packetRecv;
	$keep_going = 1;
	$lim_bytes = 1000000000; //1 GB
	
	while(($connection = socket_accept($socketRecv)) != FALSE) {
		$packet = null;
		do {
		sleep(3);
			$packetRecv = socket_read($connection, $lim_bytes, PHP_BINARY_READ);
			$packet = new Pacote;
			$packet->convert($packetRecv);
			
			//socket pra enviar pra camada de aplicação
			$socketSend = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die(socket_strerror(socket_last_error())."\n");
			echo "Socket criado\n";

			$result = socket_connect($socketSend, $hostSend, $portSend) or die(socket_strerror(socket_last_error($socketSend))."\n");
			echo "Conectado a $hostSend.\n";
			
			//se for o do hanshake
			if($packet->flags == "000010") {
				$ack = clone $packet;
				$ack->set($portSend, $port, "ACK", "oi");
				socket_write($connection, $ack->toString(), strlen($ack->toString()));

				$resposta = socket_read($socketRecv, $lim_bytes, PHP_BINARY_READ);
				$ackClient = new Pacote;
				$ackClient->convert($resposta);
				continue;
			}

			$unpacked = $packet->dado;
			if(socket_write($socketSend, $unpacked, strlen($unpacked)) === false) echo "Erro de envio para aplicação";

			$respApp = socket_read($socketSend, $lim_bytes, PHP_BINARY_READ);

			$packet2 = new Pacote;
			$packet2->set($portSend, $port, "", $respApp);

			if(socket_write($connection, $packet2->toString(), strlen($packet2->toString())) === false) echo "Erro de envio para transporte";
			$keep_going ++;
		}// while ($packet->flags != "000001" && $packet->flags != "000000");
 while ($keep_going < 5);
		//socket_close($connection);
	}

	//socket_close($socketRecv);
?>
