<?php
require_once("pacote.php");
	
	$host = "127.0.0.1";
	$port = "20000";
	$hostSend = "127.0.0.1";
	$portSend = 10001;
	
	$socketRecv = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
	$lim_bytes = 1000000000; //1 GB
	
	socket_bind($socketRecv, $host, $port);
	
	socket_listen($socketRecv, SOMAXCONN);
	echo "Esperando a camada de rede\n";

	$connection;
	$packetRecv;
	$keep_going;
	while(($connection = socket_accept($socketRecv)) != FALSE) {
		$packet = null;
		do {
			$packetRecv = socket_read($connection, $lim_bytes, PHP_BINARY_READ);
			$packet = new Pacote;
			$packet->convert($packetRecv);
			var_dump($packet);			
			if($packet->flags == "000010") {
				$ack = clone $packet;
				$ack->set($portSend, $port, "ACK", "oi");
				socket_write($socketRecv, $ack->toString(), strlen($ack->toString()));
				
				$ouvindo = socket_accept($socket);
				$resposta = socket_read($ouvindo, $lim_bytes, PHP_BINARY_READ);
				$ackClient = new Pacote;
				$ackClient->convert($resposta);
				continue;
			}
			
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
			if(socket_write($socketSend, $packet->toString(), strlen($packet->toString())) === false) echo "Erro de envio para aplicação";
			else echo "Enviado: ".$packet->toString();
		} while ($packet->flags != "000001" && $packet->flags != "000000");

		socket_close($connection);
	}
	
	socket_close($socketRecv);
?> 


