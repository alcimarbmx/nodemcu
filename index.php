<?php
	include 'conexao.php';

$n_pag = 25;
if($_GET):
$pag = intval($_GET['pag']);
else:
	$pag = 0;
endif;
	$data = date("Y-m");
	if($_SERVER['REQUEST_METHOD'] == "POST"){
		if(!empty($_POST['data'])){
			$data = $_POST['data'];
			$expData = explode("/", $data);
			//$data = $expData[2]."/".$expData[1]."/".$expData[0];
		}
		$sql = "SELECT * FROM dados WHERE horario LIKE '%{$data}%' ORDER BY horario DESC";
	}else{
		$sql = "SELECT * FROM dados WHERE horario LIKE '%{$data}%' ORDER BY horario DESC LIMIT $pag, $n_pag";
	}
	$stmt = $pdo->prepare($sql);
	$stmt->execute();

	//$sql_count = "SELECT* FROM dados";
	$linha = $stmt->rowCount();
	$paginas = ceil($linha / $n_pag);
	echo $paginas."teste";
	?>

<!DOCTYPE html>
<html lang="pt-br">
<head>
	<meta charset="utf8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
	<title>Estação NodeMcu</title>
	<link rel="icon" href="icon.png">
	<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0-beta/css/materialize.min.css">
	<script src = "https://cdnjs.cloudflare.com/ajax/libs/materialize/0.97.3/js/materialize.min.js">
      </script>

	<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css" integrity="sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO" crossorigin="anonymous">
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
</head>
<body class="light darken-2">

<div class="container">
<div class="card-panel accent-2">

<h1>Mini estação NodeMcu <img rel="icon" src="icon.png"></h1>

</div>

<form action="" method="post" class="pure-form">

	<div  class="input-field col s12">
		<input type="date" name="data">
	</div>

	<input class="btn" type="submit" name="submit" value="Buscar">
</form>
<a href="?pag=<?php  echo $pag - 25;?>" > << </a>
<a href="?pag=<?php  echo $pag + 25;?>" > >> </a>


<hr  class="featurette-divider">

<table class="bordered striped centered">

	<thead>
	<tr>
		<th>id</th>
		<th>Luminosidade</th>
		<th>Temperatura</th>
		<th>Umidade</th>
		<th>Temp. Solo</th>
		<th>Temperatura BMP</th>
		<th>Pressão</th>
		<th>Altitude</th>
		<th>P. Mar</th>
		<th>Data/Hora</th>
	</tr>
	</thead>
	<tbody>


	<?php
	 $index = 0;
	while($row = $stmt->fetch(PDO::FETCH_OBJ)){
		$formatData = strtotime($row->horario);
		$data = date("d/m/Y H:i:s", $formatData);
	?>

		<tr>
			<td><?php echo $index++;?></td>
			<td><?php echo $row->id;?></td>
			<td><?php echo $row->luminosidade;?></td>
			<td><?php	echo $row->temperatura.' ºC';?></td>
			<td><?php	echo $row->umidade.' %'; ?></td>
			<td><?php	echo $row->tsolo.' ºC'; ?></td>
			<td><?php	echo $row->temp.' ºC'; ?></td>
			<td><?php	echo $row->press.' Pa'; ?></td>
			<td><?php	echo $row->altitude.' M'; ?></td>
			<td><?php	echo $row->pressMar.' Pa'; ?></td>
			<td><?php	echo $data; }?></td>
		</tr>
	</tbody>
	</table>

	<hr class="featurette-divider">
</div>
</body>
</html>
