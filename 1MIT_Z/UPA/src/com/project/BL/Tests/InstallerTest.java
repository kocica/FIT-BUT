package com.project.BL.Tests;

import com.project.BL.Base.*;
import org.junit.jupiter.api.*;

class InstallerTest {

    @Test
    void install() throws Exception {
        var installer = new Installer();
        installer.Install();
    }
}