import os
import subprocess
import getpass
import npyscreen


class FormOne(npyscreen.FormBaseNew):

    def create(self):
        self.address = self.add(npyscreen.TitleText, name="Address:")
        self.private_key = self.add(npyscreen.TitleText, name="Private Key:")
        self.dns = self.add(npyscreen.TitleText, name="DNS:")
        self.public_key = self.add(npyscreen.TitleText, name="Public Key:")
        self.allowed_ips = self.add(npyscreen.TitleText, name="Allowed IPs:")
        self.endpoint = self.add(npyscreen.TitleText, name="Endpoint:")
        self.keepalive = self.add(npyscreen.TitleText, name="Keepalive:")
        self.save_button = self.add(npyscreen.ButtonPress, name="Save")
        self.save_button.whenPressed = self.save_config

    def save_config(self):
        address = self.address.value
        private_key = self.private_key.value
        dns = self.dns.value
        public_key = self.public_key.value
        allowed_ips = self.allowed_ips.value
        endpoint = self.endpoint.value
        keepalive = self.keepalive.value

        if not address or not private_key or not public_key or not allowed_ips or not endpoint or not keepalive:
            result = npyscreen.notify_yes_no("Some fields are empty. Do you want to save the partial configuration?",
                                             title="Confirmation")
            if not result:
                return

        config = "[Interface]\n"
        if address:
            config += "Address = " + address + "\n"
        if private_key:
            config += "PrivateKey = " + private_key + "\n"
        if dns:
            config += "DNS = " + dns + "\n"
        config += "\n"

        if public_key or allowed_ips or endpoint or keepalive:
            config += "[Peer]\n"

            if public_key:
                config += "PublicKey = " + public_key + "\n"
            if allowed_ips:
                config += "AllowedIPs = " + allowed_ips + "\n"
            if endpoint:
                config += "Endpoint = " + endpoint + "\n"
            if keepalive:
                config += "PersistentKeepalive = " + keepalive + "\n"

        configs_dir = os.path.join(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")), "configs")
        if not os.path.exists(configs_dir):
            os.makedirs(configs_dir)

        file_number = self.generate_file_number()
        file_name = f"wireguard_{file_number}.conf"
        file_path = os.path.join(configs_dir, file_name)

        with open(file_path, "w") as f:
            f.write(config)

        npyscreen.notify_confirm(f"Config saved to {file_path}", title="Success")

    def generate_file_number(self):
        configs_dir = os.path.join(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")), "configs")

        file_number = 1
        while os.path.exists(os.path.join(configs_dir, f"wireguard_{file_number}.conf")):
            file_number += 1

        return file_number


class FormTwo(npyscreen.FormBaseNew):
    def create(self):
        self.config_file = self.add(npyscreen.TitleText, name="Path to .conf file:")
        self.password = self.add(npyscreen.TitlePassword, name="Password:")
        self.output = self.add(npyscreen.BoxTitle, name="Output:", max_height=10, editable=False)
        self.connect_button = self.add(npyscreen.ButtonPress, name="Connect")
        self.connect_button.whenPressed = self.connect_wg_quick
        self.disconnect_button = self.add(npyscreen.ButtonPress, name="Disconnect")
        self.disconnect_button.whenPressed = self.disconnect_wg_quick

    def run_command_with_password(self, command, password):
        sudo_command = f"echo '{password}' | sudo -S {command}"
        try:
            output = subprocess.check_output(sudo_command, shell=True, stderr=subprocess.STDOUT)
            return output.decode()
        except subprocess.CalledProcessError as e:
            return e.output.decode()

    def connect_wg_quick(self):
        config_path = self.config_file.value
        password = self.password.value

        # Проверяем наличие активного подключения
        check_command = "wg show"
        try:
            check_output = self.run_command_with_password(check_command, password)
        except subprocess.CalledProcessError:
            check_output = ""

        if "interface" in check_output:
            # Если есть активное подключение, предлагаем его отключить
            npyscreen.notify_confirm(
                "There is an active WireGuard connection. Disconnect it before establishing a new connection.",
                title="Warning")
        else:
            # Если нет активного подключения, устанавливаем новое подключение
            command = f"wg-quick up {config_path}"
            try:
                output = self.run_command_with_password(command, password)
                result = "WireGuard connection established successfully."
            except subprocess.CalledProcessError as e:
                result = f"Error occurred while establishing WireGuard connection:\n{e.output}"

            self.output.values = result.split("\n")
            self.display()

    def disconnect_wg_quick(self):
        config_path = self.config_file.value
        password = self.password.value

        command = f"wg-quick down {config_path}"
        try:
            output = self.run_command_with_password(command, password)
            result = "WireGuard connection disconnected successfully."
        except subprocess.CalledProcessError as e:
            result = f"Error occurred while disconnecting WireGuard connection:\n{e.output}"

        self.output.values = result.split("\n")
        self.display()


# TODO Если после запуска приложения зайти сразу сюда, появится системный запрос sudo пароля. Желательно заменить интрейсным
class FormThree(npyscreen.FormBaseNew):
    def create(self):
        self.output = self.add(npyscreen.BoxTitle, name="Output:", max_height=10, editable=False)
        self.refresh_button = self.add(npyscreen.ButtonPress, name="Refresh")
        self.refresh_button.whenPressed = self.refresh_wg_show

    def beforeEditing(self):
        self.refresh_wg_show()

    def refresh_wg_show(self):
        command = "sudo wg show"
        try:
            output = subprocess.check_output(command, shell=True)
            self.output.values = output.decode().split("\n")
        except subprocess.CalledProcessError as e:
            self.output.values = [f"Error occurred while executing 'wg show': {e.output.decode()}"]

        self.display()


class FormFour(npyscreen.FormBaseNew):
    def create(self):
        self.text = self.add(npyscreen.TitleText, name="This is form four")


class FormFive(npyscreen.Form):
    def create(self):
        self.text = self.add(npyscreen.TitleText, name="This is form five")


class MenuForm(npyscreen.FormWithMenus):
    def create(self):
        ascii_art = r"""
____    __    ____  _______        .___________. __    __   __  
\   \  /  \  /   / /  _____|       |           ||  |  |  | |  | 
 \   \/    \/   / |  |  __   ______`---|  |----`|  |  |  | |  | 
  \            /  |  | |_ | |______|   |  |     |  |  |  | |  | 
   \    /\    /   |  |__| |            |  |     |  `--'  | |  | 
    \__/  \__/     \______|            |__|      \______/  |__| 
                                                                
        """
        self.add(npyscreen.TitleFixedText, name="To close the application, press Ctrl+Q", editable=False)
        self.add(npyscreen.MultiLineEdit, value=ascii_art, editable=False)
        ##выход в меню не работает почему-то
        self.add_handlers({"^Q": self.exit_app})
        menu = self.new_menu(name="Menu", shortcut="^X")
        menu.addItem(text="Generate Config", onSelect=self.switch_to_form_one)
        menu.addItem(text="Connect", onSelect=self.switch_to_form_two)
        menu.addItem(text="Connection Status", onSelect=self.switch_to_form_three)
        menu.addItem(text="Config list(WIP)", onSelect=self.switch_to_form_four)
        menu.addItem(text="Settings (WIP)", onSelect=self.switch_to_form_five)
        menu.addItem(text="Exit", onSelect=self.exit_app)

    def exit_app(self, *args, **kwargs):
        self.parentApp.switchForm(None)

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

    def on_ok(self):
        self.parentApp.switchForm(None)


class MenuApp(npyscreen.NPSAppManaged):
    def onStart(self):
        self.addForm("MAIN", MenuForm, name="WG-TUI")
        self.addForm("ONE", FormOne, name="Generate Config")
        self.addForm("TWO", FormTwo, name="Connect")
        self.addForm("THREE", FormThree, name="Connection Status")
        self.addForm("FOUR", FormFour, name="Config list(WIP)")
        self.addForm("FIVE", FormFive, name="Settings (WIP)")


if __name__ == "__main__":
    app = MenuApp()
    app.run()
