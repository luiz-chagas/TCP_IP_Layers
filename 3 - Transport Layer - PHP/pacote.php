<?php
 
class Pacote {
	public $portaOrigem = "00000000000000000"; //16
	public $portaDestino = "00000000000000000"; //16
	public $sequencia = "0000000000000000000000000000000000"; //32
	public $ack = "0000000000000000000000000000000000"; //32
	public $offset = "0000"; //4
	public $reservado = "000000"; //6
	public $flags = "000000"; //6
	public $janela = "00000000000000000"; //16
	public $checksum = "00000000000000000"; //16
	public $urgente = "00000000000000000"; //16
	public $opcoes = "";
	public $padding = "";
	public $dado = "";
	
	public function set($origem, $destino, $flag, $dados) {
		$this->setPortaOrigem($origem);
		$this->setPortaDestino($destino);
		$this->setDado(nl2br($dados));
		if($flag == "SYN") $this->setFlags("000010");
		if($flag == "ACK") $this->setFlags("010000");
		if($flag == "FIN") $this->setFlags("000001");
	}
	
	public function convert($resposta) {
		$this->portaOrigem = substr($resposta, 0, 16);
		$this->portaDestino = substr($resposta, 16, 16);
		$this->sequencia = substr($resposta, 32, 32);
		$this->ack = substr($resposta, 64, 32);
		$this->offset = substr($resposta, 96, 4);
		$this->reservado = substr($resposta, 100, 6);
		$this->flags = substr($resposta, 106, 6);
		$this->janela = substr($resposta, 112, 16);
		$this->checksum = substr($resposta, 128, 16);
		$this->urgente = substr($resposta, 144, 16);
		$this->dado = substr($resposta, 224);
	}
	
	public function toString() {
		return $this->portaOrigem."\n".$this->portaDestino."\n".$this->sequencia."\n".$this->ack."\n".$this->offset."\n".$this->reservado."\n".$this->flags."\n".$this->janela."\n".$this->checksum."\n".$this->urgente."\n".$this->dado;
	}
	
	public function setPortaOrigem($newval) {
    	$this->portaOrigem = $newval;
	}
	public function setPortaDestino($newval) {
    	$this->portaDestino = $newval;
	}
	public function setSequencia($newval) {
    	$this->sequencia = $newval;
	}
	public function setAck($newval) {
    	$this->ack = $newval;
	}
	public function setOffset($newval) {
    	$this->offset = $newval;
	}
	public function setReservado($newval) {
    	$this->reservado = $newval;
	}
	public function setFlags($flag) {
		if($flag == "SYN") $this->flags = 000010;
		if($flag == "ACK") $this->flags = 010000;
		if($flag == "FIN") $this->flags = 000001;
	}
	public function setJanela($newval) {
    	$this->janela = $newval;
	}
	public function setChecksum($newval) {
    	$this->checksum = $newval;
	}
	public function setUrgente($newval) {
    	$this->urgente = $newval;
	}
	public function setOpcoes($newval) {
    	$this->opcoes = $newval;
	}
	public function setPadding($newval) {
    	$this->padding = $newval;
	}
	public function setDado($newval) {
    	$this->dado = $newval;
	}
}

?>
