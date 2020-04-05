<?php include('connection.php');
?>
<?php
	$id = 0;
	$statuss = 0;
	$result = mysqli_query($conn,"UPDATE rak SET id = '$id', statuss='$statuss' WHERE id=$id");
?>