pipeline {
    agent {
        dockerfile {
              filename 'Dockerfile.build'
        }
    }
    stages {
        stage('Build') {
            steps {
                sh 'pwd'
                sh 'git submodule update --init --recursive'
                sh 'cd qasm_flex_bison && mkdir build && cd build && cmake ../library && make'
            }
        }
        stage('Test') {
            steps {
               sh 'make test'

            }
        }

    }
}
