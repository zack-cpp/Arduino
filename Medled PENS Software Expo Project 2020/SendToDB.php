<?php include_once('connection.php');
?>
<?php
	$ssql = "SELECT * FROM rak Limit 10";
	$query = mysqli_query($conn,$ssql);
	while($row = mysqli_fetch_array($query)){
		echo $row['statuss'].",";
	}
?>