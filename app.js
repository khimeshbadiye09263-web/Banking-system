async function api(url, data) {
  let res = await fetch(url, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(data),
  });
  return res.json();
}

async function createAccount() {
  let result = await api("/create", {
    name: document.getElementById("name").value,
    acc: parseInt(document.getElementById("acc").value),
    balance: parseFloat(document.getElementById("balance").value),
  });
  alert(result.message);
}

async function deposit() {
  let result = await api("/deposit", {
    acc: parseInt(document.getElementById("dacc").value),
    amount: parseFloat(document.getElementById("damount").value),
  });
  alert(result.message);
}

async function withdraw() {
  let result = await api("/withdraw", {
    acc: parseInt(document.getElementById("wacc").value),
    amount: parseFloat(document.getElementById("wamount").value),
  });
  alert(result.message);
}

async function checkBalance() {
  let result = await api("/balance", {
    acc: parseInt(document.getElementById("bacc").value),
  });
  document.getElementById("balanceResult").innerText =
    "Balance: ₹" + result.balance;
}
