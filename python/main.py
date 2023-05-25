import subprocess

import npyscreen

class FormOne(npyscreen.Form):

    def create(self):
        # Добавляем поля для ввода данных
        self.address = self.add(npyscreen.TitleText, name="Address:")
        self.private_key = self.add(npyscreen.TitleText, name="Private Key:")
        self.dns = self.add(npyscreen.TitleText, name="DNS:")
        self.public_key = self.add(npyscreen.TitleText, name="Public Key:")
        self.allowed_ips = self.add(npyscreen.TitleText, name="Allowed IPs:")
        self.endpoint = self.add(npyscreen.TitleText, name="Endpoint:")
        #Должно быть Persistent Keepalive, но тогда ввод переезжает под заголовок. Если найдете как пофиксить, верните полное имя.
        self.keepalive = self.add(npyscreen.TitleText, name="Keepalive:")
        # Добавляем кнопку для сохранения данных в файл
        self.save_button = self.add(npyscreen.ButtonPress, name="Save")
        # Устанавливаем функцию, которая будет вызвана при нажатии на кнопку
        self.save_button.whenPressed = self.save_config

    def save_config(self):
        # Получаем значения из полей ввода
        address = self.address.value
        private_key = self.private_key.value
        dns = self.dns.value
        public_key = self.public_key.value
        allowed_ips = self.allowed_ips.value
        endpoint = self.endpoint.value
        keepalive = self.keepalive.value
        # Формируем строку с конфигурацией wireguard
        config = "[Interface]\n"
        config += "Address = " + address + "\n"
        config += "PrivateKey = " + private_key + "\n"
        config += "DNS = " + dns + "\n"
        config += "\n"
        config += "[Peer]\n"
        config += "PublicKey = " + public_key + "\n"
        config += "AllowedIPs = " + allowed_ips + "\n"
        config += "Endpoint = " + endpoint + "\n"
        config += "PersistentKeepalive = " + keepalive + "\n"
        # Открываем файл для записи в той же папке, где находится питон-файл
        with open("wireguard.conf", "w") as f:
            # Записываем конфигурацию в файл
            f.write(config)
            # Выводим сообщение об успешном сохранении
            npyscreen.notify_confirm("Config saved to wireguard.conf", title="Success")

##Во второй форме реализован пример запуска команды в терминале, в которую подставляется ввод пользователя. После выполнения команды выводится ее результат, но мы потом сможем выводить уже его интерпретацию
class FormTwo(npyscreen.Form):
    def create(self):
        self.argument = self.add(npyscreen.TitleText, name="Argument:")
        self.output = self.add(npyscreen.BoxTitle, name="Output:", max_height=10, editable=False)
        self.run_button = self.add(npyscreen.ButtonPress, name="Run")
        self.run_button.whenPressed = self.run_ls

    def run_ls(self):
        arg = self.argument.value
        command = f"ls {arg}"
        output = subprocess.run(command, shell=True, capture_output=True)
        if output.returncode == 0:
            result = output.stdout.decode()
        else:
            result = output.stderr.decode()
        self.output.values = result.split("\n")
        self.display()
class FormThree(npyscreen.Form):
    def create(self):
        self.text = self.add(npyscreen.TitleText, name="This is form three")

class FormFour(npyscreen.Form):
    def create(self):
        self.text = self.add(npyscreen.TitleText, name="This is form four")

class FormFive(npyscreen.Form):
    def create(self):
        self.text = self.add(npyscreen.TitleText, name="This is form five")

class MenuForm(npyscreen.FormWithMenus):
    def create(self):
        self.text = self.add(npyscreen.TitleText, name="This is the menu form")
        ##выход в меню не работает почему-то
        menu = self.add_menu(name="Menu", shortcut="^M")
        menu.addItem(text="Form One", onSelect=self.switch_to_form_one)
        menu.addItem(text="Ls command", onSelect=self.switch_to_form_two)
        menu.addItem(text="Form Three", onSelect=self.switch_to_form_three)
        menu.addItem(text="Form Four", onSelect=self.switch_to_form_four)
        menu.addItem(text="Form Five", onSelect=self.switch_to_form_five)
        menu.addItem(text="Exit", onSelect=self.exit_app)

    def switch_to_form_one(self):
        self.parentApp.switchForm("ONE")

    def switch_to_form_two(self):
        self.parentApp.switchForm("TWO")

    def switch_to_form_three(self):
        self.parentApp.switchForm("THREE")

    def switch_to_form_four(self):
        self.parentApp.switchForm("FOUR")

    def switch_to_form_five(self):
        self.parentApp.switchForm("FIVE")

    def exit_app(self):
        self.parentApp.setNextForm(None)
        self.editing = False

class MenuApp(npyscreen.NPSAppManaged):
    def onStart(self):
        self.addForm("MAIN", MenuForm, name="Menu")
        self.addForm("ONE", FormOne, name="Form One")
        self.addForm("TWO", FormTwo, name="Form Two")
        self.addForm("THREE", FormThree, name="Form Three")
        self.addForm("FOUR", FormFour, name="Form Four")
        self.addForm("FIVE", FormFive, name="Form Five")

if __name__ == "__main__":
    app = MenuApp()
    app.run()
