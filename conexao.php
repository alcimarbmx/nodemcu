<?php
		
	try{
		$host = "localhost";
		$user = "root";
		$pass = "";
		$bd = "nodemcu";		
		$pdo = new PDO("mysql:host={$host};dbname={$bd};charset=utf8;","{$user}","{$pass}");
	}catch(PDOException $erro){
		echo "Erro de conexão: ". $erro->getMessage();
	}

?>