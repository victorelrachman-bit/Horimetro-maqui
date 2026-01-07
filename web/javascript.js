var user = {
	username: "Vectorchapa1234",
	password: "Compacdahora123"
};
var client = mqtt.connect("wss://846613f2ff8f4e40be716012473fc849.s1.eu.hivemq.cloud:8884/mqtt", user);
client.subscribe("hori/m1/cont");
client.subscribe("hori/m2/cont");
client.subscribe("hori/m3/cont");
client.subscribe("hori/m1/diz_pause");
client.subscribe("hori/m2/diz_pause");
client.subscribe("hori/m3/diz_pause");
client.on("message", read_message);

function read_message(topic, message)
{
	const agora = new Date();
	let cont = parseInt(message.toString(), 10);
	cont /= 1000;
	
	const passado = new Date(agora.getTime() - cont * 1000);
	let pas = `${passado.getMinutes().toString()}:${passado.getSeconds().toString()}`;

	const futuro = new Date(agora.getTime() +  (32 - cont) * 1000);
	let	fut = `${futuro.getMinutes().toString()}:${futuro.getSeconds().toString()}`;

	if(topic === "hori/m1/cont")
	{
		document.getElementById("ult-1").textContent = pas;
		document.getElementById("pro-1").textContent = fut;
	}
	else if (topic === "hori/m2/cont") 
	{
		document.getElementById("ult-2").textContent = pas;
		document.getElementById("pro-2").textContent = fut;
	}
	else if (topic === "hori/m3/cont") 
	{
		document.getElementById("ult-3").textContent = pas;
		document.getElementById("pro-3").textContent = fut;
	}else if(topic === "hori/m1/diz_pause")
	{
		if(message.toString() === "pausado")
			document.getElementById("p-1").textContent = "Despausar";
		else
			document.getElementById("p-1").textContent = "Pausar";
	}else if(topic === "hori/m2/diz_pause")
	{
		if(message.toString() === "pausado")
			document.getElementById("p-2").textContent = "Despausar";
		else
			document.getElementById("p-2").textContent = "Pausar";
	}else if(topic === "hori/m3/diz_pause")
	{
		if(message.toString() === "pausado")
			document.getElementById("p-3").textContent = "Despausar";
		else
			document.getElementById("p-3").textContent = "Pausar";
	}
}

function pause(num)
{
	client.publish("hori/m" + num + "/pause", "pause");
}

function reinicia(num) {
	client.publish("hori/m" + num + "/reiniciar", "reinicie");
}

var p1 = document.getElementById("p-1");
var p2 = document.getElementById("p-2");
var p3 = document.getElementById("p-3");
p1.addEventListener("click", () => pause(1));
p2.addEventListener("click", () => pause(2));
p3.addEventListener("click", () => pause(3));

var r1 = document.getElementById("r-1");
var r2 = document.getElementById("r-2");
var r3 = document.getElementById("r-3");
r1.addEventListener("click", () => reinicia(1));
r2.addEventListener("click", () => reinicia(2));
r3.addEventListener("click", () => reinicia(3));
