<?php
 
class Pacote {
	public $portaOrigem = "0000000000000000"; //16
	public $portaDestino = "0000000000000000"; //16
	public $sequencia = "00000000000000000000000000000000"; //32
	public $ack = "00000000000000000000000000000000"; //32
	public $offset = "0000"; //4
	public $reservado = "000000"; //6
	public $flags = "000000"; //6
	public $janela = "0000000000000000"; //16
	public $checksum = "0000000000000000"; //16
	public $urgente = "0000000000000000"; //16
	public $opcoes = "00000000000000000000000000000000";
	public $padding = "00000000000000000000000000000000";
	public $dado = "";
	
	public function set($origem, $destino, $flag, $dados) {
		$this->setPortaOrigem(str_pad($origem, 16, "0", STR_PAD_LEFT));
		$this->setPortaDestino(str_pad($destino, 16, "0", STR_PAD_LEFT));
		$this->setDado(str_replace(array("\n\r", "\n", "\r"), '', nl2br($dados)));
		if($flag == "SYN") $this->setFlags("000010");
		if($flag == "ACK") $this->setFlags("010000");
		if($flag == "FIN") $this->setFlags("000001");
	}
	
	public function convert($resposta) {
		//$resposta = br2nl($resposta);
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
		$this->padding = substr($resposta, 160, 32);
		$this->padding = substr($resposta, 92, 32);
		$this->dado = substr($resposta, 224);
	}
	
	public function toString() {
		return $this->portaOrigem.$this->portaDestino.$this->sequencia.$this->ack.$this->offset.$this->reservado.$this->flags.$this->janela.$this->checksum.$this->urgente.$this->dado."\n";
	}
	
	public function toString2() {
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
		$this->flags = $flag;
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
