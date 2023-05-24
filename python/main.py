import subprocess

import npyscreen

class FormOne(npyscreen.Form):
    def create(self):
        self.text = self.add(npyscreen.TitleText, name="This is form one")

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
