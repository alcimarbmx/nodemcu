<?php
	include "conexao.php";

	$luminosidade = $_GET['luminosidade'];
	$temperatura = $_GET['temperatura'];
	$umidade = $_GET['umidade'];
	$tsolo = $_GET['tsolo'];
	//bmp
	$temp = $_GET['bmpTemp'];
	$press = $_GET['bmpPress'];
	$altitude = $_GET['bmpAltt'];
	$pressMar = $_GET['bmpSeaP'];

	$sql = "INSERT INTO dados (luminosidade, temperatura, umidade, tsolo, temp, press, altitude, pressMar) VALUES
	(:luminosidade, :temperatura, :umidade, :tsolo, :temp, :press, :altitude, :pressMar)";

	$stmt = $pdo->prepare($sql);

	$stmt->BindParam(':luminosidade', $luminosidade);
	$stmt->BindParam(':temperatura', $temperatura);
	$stmt->BindParam(':umidade', $umidade);
	$stmt->BindParam(':tsolo', $tsolo);
	$stmt->BindParam(':temp', $temp);
	$stmt->BindParam(':press', $press);
	$stmt->BindParam(':altitude', $altitude);
	$stmt->BindParam(':pressMar', $pressMar);

	if($stmt->execute()):
		echo "salvo_com_sucesso";
	else:
		echo "erro_ao_salvar";
	endif;


	/*
	http://localhost/nodemcu/salvar.php?sensor1=121&sensor2=4545&sensor3=34343

	*/
?>
